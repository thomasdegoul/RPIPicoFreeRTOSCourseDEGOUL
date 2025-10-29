/**
 * Demo program to flash 5 LEDs (1 BlinkAgent + 4 BlinkWorkers) using FreeRTOS tasks and a counting semaphore.
 * The semaphore is initialized with 2 tokens, so only 2 workers can blink their LEDs simultaneously.
 * Thomas DEGOUL (F14218804)
 * 28-Oct-2025
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

// Classes pour gérer les LEDs (à adapter selon tes fichiers BlinkAgent.h/BlinkWorker.h)
class BlinkAgent {
public:
    BlinkAgent(uint led_pad) : led_pad(led_pad) {
        gpio_init(led_pad);
        gpio_set_dir(led_pad, GPIO_OUT);
    }
    static void task(void *params) {
        BlinkAgent *agent = (BlinkAgent *)params;
        while (true) {
            gpio_put(agent->led_pad, 1);
            vTaskDelay(500);
            gpio_put(agent->led_pad, 0);
            vTaskDelay(500);
        }
    }
    void start(const char *name, UBaseType_t priority) {
        xTaskCreate(task, name, 256, this, priority, NULL);
    }
private:
    uint led_pad;
};

class BlinkWorker {
public:
    BlinkWorker(uint led_pad) : led_pad(led_pad), sem(NULL) {
        gpio_init(led_pad);
        gpio_set_dir(led_pad, GPIO_OUT);
    }
    void setSemaphore(SemaphoreHandle_t s) { sem = s; }
    static void task(void *params) {
        BlinkWorker *worker = (BlinkWorker *)params;
        while (true) {
            // Attend un token disponible
            printf("%s: En attente d'un token...\n", pcTaskGetName(NULL));
            xSemaphoreTake(worker->sem, portMAX_DELAY);
            printf("%s: Token acquis ! LED ON\n", pcTaskGetName(NULL));

            // Allume la LED
            gpio_put(worker->led_pad, 1);
            vTaskDelay(1000);  // LED allumée 1 seconde

            // Éteint la LED
            gpio_put(worker->led_pad, 0);
            printf("%s: Token libéré. LED OFF\n", pcTaskGetName(NULL));
            xSemaphoreGive(worker->sem);

            vTaskDelay(500);  // Délai avant la prochaine tentative
        }
    }
    void start(const char *name, UBaseType_t priority) {
        xTaskCreate(task, name, 256, this, priority, NULL);
    }
private:
    uint led_pad;
    SemaphoreHandle_t sem;
};

// Priorité standard
#define TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
// GPIOs pour les LEDs
#define LED_PAD      0
#define LED1_PAD     2
#define LED2_PAD     3
#define LED3_PAD     4
#define LED4_PAD     5

void runTimeStats() {
    TaskStatus_t *pxTaskStatusArray;
    UBaseType_t uxArraySize = uxTaskGetNumberOfTasks();
    printf("\n--- État des tâches ---\n");
    printf("Nombre de tâches: %d\n", uxArraySize);

    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
    if (pxTaskStatusArray != NULL) {
        uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, NULL);
        for (UBaseType_t i = 0; i < uxArraySize; i++) {
            printf("Tâche: %s \t Pri:%d \t Stack restante:%d\n",
                   pxTaskStatusArray[i].pcTaskName,
                   pxTaskStatusArray[i].uxCurrentPriority,
                   pxTaskStatusArray[i].usStackHighWaterMark);
        }
        vPortFree(pxTaskStatusArray);
    }

    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);
    printf("HEAP disponible: %d octets\n", heapStats.xAvailableHeapSpaceInBytes);
}

void mainTask(void *params) {
    // 1 BlinkAgent (clignote librement) + 4 BlinkWorkers (limités par le sémaphore)
    BlinkAgent blink(LED_PAD);
    BlinkWorker worker1(LED1_PAD);
    BlinkWorker worker2(LED2_PAD);
    BlinkWorker worker3(LED3_PAD);
    BlinkWorker worker4(LED4_PAD);

    // Crée un sémaphore comptant avec 2 tokens max (initialisés à 2)
    SemaphoreHandle_t sem = xSemaphoreCreateCounting(2, 2);
    worker1.setSemaphore(sem);
    worker2.setSemaphore(sem);
    worker3.setSemaphore(sem);
    worker4.setSemaphore(sem);

    printf("Démarrage des tâches avec 4 workers et 2 tokens...\n");
    blink.start("BlinkAgent", TASK_PRIORITY);
    worker1.start("Worker1", TASK_PRIORITY);
    worker2.start("Worker2", TASK_PRIORITY);
    worker3.start("Worker3", TASK_PRIORITY);
    worker4.start("Worker4", TASK_PRIORITY);

    while (true) {
        runTimeStats();
        vTaskDelay(3000);
    }
}

void vLaunch() {
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);
    vTaskStartScheduler();
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    printf("GO: Test du sémaphore comptant (4 workers, 2 tokens)\n");
    vLaunch();
    return 0;
}

/**
 * Demo program to flash LEDs attached to GPIO PADs using FreeRTOS tasks and a semaphore.
 * Jon Durrant (adapted)
 * 28-Oct-2025
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include "BlinkAgent.h"
#include "BlinkWorker.h"

// Standard Task priority
#define TASK_PRIORITY        ( tskIDLE_PRIORITY + 1UL )
// LED PADs to use
#define LED_PAD              0
#define LED1_PAD             2
#define LED2_PAD             3
#define LED3_PAD             4
#define LED4_PAD             5

void runTimeStats() {
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;

    // Get number of tasks
    uxArraySize = uxTaskGetNumberOfTasks();
    printf("Number of tasks %d\n", uxArraySize);

    // Allocate a TaskStatus_t structure for each task.
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );
    if( pxTaskStatusArray != NULL ) {
        // Generate raw status information about each task.
        uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

        // Print stats
        for( x = 0; x < uxArraySize; x++ ) {
            printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
                pxTaskStatusArray[ x ].xTaskNumber,
                pxTaskStatusArray[ x ].uxCurrentPriority,
                pxTaskStatusArray[ x ].uxBasePriority,
                pxTaskStatusArray[ x ].usStackHighWaterMark,
                pxTaskStatusArray[ x ].pcTaskName
            );
        }
        // Free array
        vPortFree( pxTaskStatusArray );
    } else {
        printf("Failed to allocate space for stats\n");
    }

    // Get heap allocation information
    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);
    printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
        heapStats.xAvailableHeapSpaceInBytes,
        heapStats.xNumberOfFreeBlocks,
        heapStats.xNumberOfSuccessfulAllocations,
        heapStats.xNumberOfSuccessfulFrees
    );
}

/**
 * Main task to blink external LEDs using workers and a semaphore.
 * @param params - unused
 */
void mainTask(void *params) {
    BlinkAgent blink(LED_PAD);
    BlinkWorker worker1(LED1_PAD);
    BlinkWorker worker2(LED2_PAD);
    BlinkWorker worker3(LED3_PAD);
    BlinkWorker worker4(LED4_PAD);

    // Create a counting semaphore with a maximum count of 2 and initial count of 2
    SemaphoreHandle_t sem = xSemaphoreCreateCounting(2, 2);

    // Assign the semaphore to each worker
    worker1.setSemaphore(sem);
    worker2.setSemaphore(sem);
    worker3.setSemaphore(sem);
    worker4.setSemaphore(sem);

    printf("Main task started\n");

    // Start all tasks
    blink.start("Blink", TASK_PRIORITY);
    worker1.start("Worker 1", TASK_PRIORITY);
    worker2.start("Worker 2", TASK_PRIORITY);
    worker3.start("Worker 3", TASK_PRIORITY);
    worker4.start("Worker 4", TASK_PRIORITY);

    while (true) { // Loop forever
        runTimeStats();
        vTaskDelay(3000);
    }
}

/**
 * Launch the tasks and scheduler
 */
void vLaunch( void ) {
    // Start main task
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);

    // Start the tasks and timer running.
    vTaskStartScheduler();
}

/**
 * Main
 * @return
 */
int main( void ) {
    // Setup serial over USB and give a few seconds to settle before we start
    stdio_init_all();
    sleep_ms(2000);
    printf("GO\n");

    // Start tasks and scheduler
    const char *rtos_name = "FreeRTOS";
    printf("Starting %s on core 0:\n", rtos_name);
    vLaunch();

    return 0;
}


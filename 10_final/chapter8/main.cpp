/**
 * Demo FreeRTOS SMP - 7 LEDs Counter
 * Jon Durrant (Modifié pour 7 LEDs)
 */

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <math.h>
#include "hardware/structs/sio.h"

#include "BlinkAgent.h"
#include "CounterAgent.h"

#define TASK_PRIORITY       ( tskIDLE_PRIORITY + 1UL )

// Broches pour les LEDs de clignotement simple
#define LED_PAD_C0          0   // Core 0
#define LED_PAD_C1          15  // Core 1

// Broches pour le compteur (7 LEDs)
#define C_LED1              2
#define C_LED2              3
#define C_LED3              4
#define C_LED4              5
#define C_LED5              6
#define C_LED6              7
#define C_LED7              8

void runTimeStats() {
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;

    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

    if( pxTaskStatusArray != NULL ){
        uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );
        for( x = 0; x < uxArraySize; x++ ) {
            printf("Task: %s \t Core: %d \t StackHW: %d\n",
                    pxTaskStatusArray[x].pcTaskName,
                    pxTaskStatusArray[x].xTaskNumber, // Note: Simplifié pour l'exemple
                    pxTaskStatusArray[x].usStackHighWaterMark);
        }
        vPortFree( pxTaskStatusArray );
    }

    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);
    printf("HEAP: %d bytes free\n", heapStats.xAvailableHeapSpaceInBytes);
}

void mainTask(void *params){
    BlinkAgent blink0(LED_PAD_C0);
    BlinkAgent blink1(LED_PAD_C1);
    // Initialisation avec les 7 broches
    CounterAgent counter(C_LED1, C_LED2, C_LED3, C_LED4, C_LED5, C_LED6, C_LED7);

    printf("Main task started\n");

    blink0.start("Blink 0", TASK_PRIORITY);
    blink1.start("Blink 1", TASK_PRIORITY);
    counter.start("Counter", TASK_PRIORITY);

    // Affinité CORE 1
    UBaseType_t coreMask1 = 0x2;
    vTaskCoreAffinitySet( blink1.getTask(), coreMask1 );
    vTaskCoreAffinitySet( counter.getTask(), coreMask1 );

    // Affinité CORE 0
    UBaseType_t coreMask0 = 0x1;
    vTaskCoreAffinitySet( blink0.getTask(), coreMask0 );

    while (true) { 
        runTimeStats();
        printf("Main on Core %ld\n", sio_hw->cpuid);

        // On utilise 0x7F (127 en décimal) pour piloter 7 bits
        uint8_t r = rand() & 0x7F; 
        counter.on(r);
        
        vTaskDelay(3000);
    }
}

void vLaunch(void) {
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 1024, NULL, TASK_PRIORITY, &task);
    vTaskStartScheduler();
}

int main(void) {
    stdio_init_all();
    sleep_ms(2000);
    printf("Système prêt - Mode 7 LEDs\n");
    vLaunch();
    return 0;
}
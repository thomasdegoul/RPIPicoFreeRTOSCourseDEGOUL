/**
 * Demo program to flash 8 LED attached to GPIO PAD 0, 2, 3, 4, 5, 6, 7, 8.
 * Uses FreeRTOS Task
 * Jon Durrant (modified bye thomas Degoul F14218804)
 * 22-Oct-2025
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "BlinkAgent.h"

// Standard Task priority
#define TASK_PRIORITY        ( tskIDLE_PRIORITY + 1UL )

// LED PAD to use
#define LED_PAD              0
#define LED1_PAD             2
#define LED2_PAD             3
#define LED3_PAD             4
#define LED4_PAD             5
#define LED5_PAD             6
#define LED6_PAD             7
#define LED7_PAD             8

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
        uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                                     uxArraySize,
                                     &ulTotalRunTime );
        // Print stats
        for( x = 0; x < uxArraySize; x++ ) {
            printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
                pxTaskStatusArray[ x ].xTaskNumber ,
                pxTaskStatusArray[ x ].uxCurrentPriority ,
                pxTaskStatusArray[ x ].uxBasePriority ,
                pxTaskStatusArray[ x ].usStackHighWaterMark ,
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
 * Main task to blink external LEDs
 * @param params - unused
 */
void mainTask(void *params) {
    BlinkAgent blink(LED_PAD);
    BlinkAgent worker1(LED1_PAD);
    BlinkAgent worker2(LED2_PAD);
    BlinkAgent worker3(LED3_PAD);
    BlinkAgent worker4(LED4_PAD);
    BlinkAgent worker5(LED5_PAD);
    BlinkAgent worker6(LED6_PAD);
    BlinkAgent worker7(LED7_PAD);

    printf("Main task started\n");

    blink.start("Blink", TASK_PRIORITY);
    worker1.start("Worker 1", TASK_PRIORITY);
    worker2.start("Worker 2", TASK_PRIORITY + 1);
    worker3.start("Worker 3", TASK_PRIORITY + 2);
    worker4.start("Worker 4", TASK_PRIORITY + 3);
    worker5.start("Worker 5", TASK_PRIORITY + 4);
    worker6.start("Worker 6", TASK_PRIORITY + 5);
    worker7.start("Worker 7", TASK_PRIORITY + 6);

    while (true) { // Loop forever
        runTimeStats();
        vTaskDelay(3000);
    }
}

/**
 * Launch the tasks and scheduler
 */
void vLaunch( void) {
    // Start blink task
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

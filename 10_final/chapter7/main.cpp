/**
 * Demo program to light 7 LEDs as binary random value
 * Uses FreeRTOS Task
 * Modifié pour 7 LEDs
 */

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <math.h>

#include "BlinkAgent.h"
#include "CounterAgent.h"
#include "DecoderAgent.h"

// Standard Task priority
#define TASK_PRIORITY       ( tskIDLE_PRIORITY + 1UL )

// LED PADs (Exemples de broches GPIO pour le Raspberry Pi Pico)
#define LED_PAD             0
#define LED1_PAD            2
#define LED2_PAD            3
#define LED3_PAD            4
#define LED4_PAD            5
#define LED5_PAD            15
#define LED6_PAD            16
#define LED7_PAD            17

void runTimeStats(){
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;

    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

    if( pxTaskStatusArray != NULL ){
        uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

        for( x = 0; x < uxArraySize; x++ ) {
            printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
                   pxTaskStatusArray[ x ].xTaskNumber ,
                   pxTaskStatusArray[ x ].uxCurrentPriority ,
                   pxTaskStatusArray[ x ].uxBasePriority ,
                   pxTaskStatusArray[ x ].usStackHighWaterMark ,
                   pxTaskStatusArray[ x ].pcTaskName );
        }
        vPortFree( pxTaskStatusArray );
    }

    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);
    printf("HEAP avl: %d, blocks %d\n", heapStats.xAvailableHeapSpaceInBytes, heapStats.xNumberOfFreeBlocks);
}

void mainTask(void *params){
    char line[80];
    BlinkAgent blink(LED_PAD);
    // Instance avec 7 pads
    CounterAgent counter(LED1_PAD, LED2_PAD, LED3_PAD, LED4_PAD, LED5_PAD, LED6_PAD, LED7_PAD);
    DecoderAgent decoder(&counter);

    printf("Main task started\n");

    blink.start("Blink", TASK_PRIORITY);
    counter.start("Counter", TASK_PRIORITY);
    decoder.start("Decode", TASK_PRIORITY);

    while (true) { 
        runTimeStats();
        // Masque pour 7 bits (0 à 127)
        uint8_t r = rand() & 0x7F; 

        sprintf(line, "{\"count\": %d}\r\n", r);
        printf("Providing Json %s\n", line);
        decoder.add(line);
        vTaskDelay(3000);
    }
}

void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);
    vTaskStartScheduler();
}

int main( void ) {
    stdio_init_all();
    sleep_ms(2000);
    printf("GO\n");

    vLaunch();
    return 0;
}
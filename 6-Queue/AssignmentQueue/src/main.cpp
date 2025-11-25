/***
 * Demo program to light 4 LEDs as binary random value.
 * Uses FreeRTOS Task
 * Jon Durrant
 * 15-Aug-2022
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <math.h>
#include "BlinkAgent.h"
#include "CounterAgent.h" // Ensure this file is included
#include <stdlib.h> // Added to ensure rand() is defined

// Standard Task priority
#define TASK_PRIORITY		( tskIDLE_PRIORITY + 1UL )
// LED PAD to use
#define LED_PAD				25
#define LED1_PAD			2
#define LED2_PAD			3
#define LED3_PAD			4
#define LED4_PAD			5


/**
 * Prints FreeRTOS runtime statistics and heap usage.
 */
void runTimeStats( ){
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	unsigned long ulTotalRunTime;

    // Get number of tasks
    uxArraySize = uxTaskGetNumberOfTasks();
    printf("Number of tasks %d\n", uxArraySize);

    // Allocate a TaskStatus_t structure for each task.
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

    if( pxTaskStatusArray != NULL ){
        // Generate raw status information about each task.
        uxArraySize = uxTaskGetSystemState( pxTaskStatusArray,
                                            uxArraySize,
                                            &ulTotalRunTime );
        
        // Print stats
        for( x = 0; x < uxArraySize; x++ )
        {
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


/***
 * Main task to control LED agents
 * @param params - unused
 */
void mainTask(void *params){
	// BlinkAgent handles the onboard LED (LED_PAD 25)
	BlinkAgent blink(LED_PAD);
	// CounterAgent handles the 4 external LEDs to display a binary value
	CounterAgent counter(LED1_PAD, LED2_PAD, LED3_PAD, LED4_PAD);
    
	printf("Main task started\n");
    
	// Start the agents as FreeRTOS tasks
	blink.start("Blink", TASK_PRIORITY);
	counter.start("Counter", TASK_PRIORITY);
    
	while (true) { // Loop forever
		runTimeStats();
        
		// Generate a random 4-bit value (0x00 to 0x0F)
		uint8_t r = rand() & 0x0F;
        
		// *** MODIFIED LINE ***
		// Now uses blink() to send the random value command to the queue,
		// instructing the CounterAgent to display 'r' in binary.
		counter.blink(r); 
        
		printf("Count R=0x%X\n", r);
        
		// Wait 3000 ms (3 seconds) before sending the NEXT command (new random number)
		vTaskDelay(3000 / portTICK_PERIOD_MS);
	}
}


/***
 * Launch the tasks and scheduler
 */
void vLaunch( void) {
	// Start main task
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 500, NULL, TASK_PRIORITY, &task);
    
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}


/***
 * Main entry point
 * @return
 */
int main( void )
{
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

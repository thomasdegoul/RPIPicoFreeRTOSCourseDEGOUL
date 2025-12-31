/***
 * Programme de démo pour allumer 7 LEDs avec une valeur binaire aléatoire
 * Utilise FreeRTOS Task
 * Jon Durrant - Modifié pour 7 LEDs
 */

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <math.h>

#include "BlinkAgent.h"
#include "CounterAgent.h"

// Priorité standard des tâches
#define TASK_PRIORITY       ( tskIDLE_PRIORITY + 1UL )

// Définition des broches (GPIO) pour les 7 LEDs + 1 LED de statut
#define LED_PAD             0  // LED de statut (BlinkAgent)
#define LED1_PAD            2
#define LED2_PAD            3
#define LED3_PAD            4
#define LED4_PAD            5
#define LED5_PAD            6
#define LED6_PAD            7
#define LED7_PAD            8

void runTimeStats(){
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    unsigned long ulTotalRunTime;

    uxArraySize = uxTaskGetNumberOfTasks();
    pxTaskStatusArray = (TaskStatus_t *)pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

    if( pxTaskStatusArray != NULL ){
      uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

      for( x = 0; x < uxArraySize; x++ ) {
          printf("Task: %d \t cPri:%d \t hw:%d \t%s\n",
                 pxTaskStatusArray[ x ].xTaskNumber ,
                 pxTaskStatusArray[ x ].uxCurrentPriority ,
                 pxTaskStatusArray[ x ].usStackHighWaterMark ,
                 pxTaskStatusArray[ x ].pcTaskName
                 );
      }
      vPortFree( pxTaskStatusArray );
    }

    HeapStats_t heapStats;
    vPortGetHeapStats(&heapStats);
    printf("HEAP avl: %d\n", heapStats.xAvailableHeapSpaceInBytes);
}

/***
 * Tâche principale
 */
void mainTask(void *params){
    // Initialisation de la LED clignotante simple
    BlinkAgent blink(LED_PAD);
    
    // Initialisation du compteur avec les 7 broches définies
    CounterAgent counter(LED1_PAD, LED2_PAD, LED3_PAD, LED4_PAD, LED5_PAD, LED6_PAD, LED7_PAD);

    printf("Main task started - 7 LEDs Mode\n");

    blink.start("Blink", TASK_PRIORITY);
    counter.start("Counter", TASK_PRIORITY);

    while (true) { 
        runTimeStats();
        
        // Masque 0x7F (0111 1111 en binaire) pour 7 bits (0 à 127)
        uint8_t r = rand() & 0x7F; 
        
        counter.blink(r);
        printf("Blinking R=0x%X (%d en decimal)\n", r, r);
        
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

/***
 * Lancement des tâches et de l'ordonnanceur
 */
void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(mainTask, "MainThread", 1024, NULL, TASK_PRIORITY, &task);
    vTaskStartScheduler();
}

int main( void ) {
    stdio_init_all();
    sleep_ms(2000);
    printf("GO - 7 LEDs Config\n");

    vLaunch();
    return 0;
}
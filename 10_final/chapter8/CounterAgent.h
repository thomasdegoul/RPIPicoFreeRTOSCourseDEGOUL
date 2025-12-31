#ifndef COUNTERAGENT_H_
#define COUNTERAGENT_H_

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Agent.h"

#define COUNT_LEDS          7  // Modifié de 4 à 7
#define COUNT_QUEUE_LEN     10
#define COUNT_BLINK_DELAY   500

class CounterAgent : public Agent {
public:
    // Constructeur mis à jour pour 7 arguments
    CounterAgent(uint8_t gp1=0, uint8_t gp2=0, uint8_t gp3=0, 
                 uint8_t gp4=0, uint8_t gp5=0, uint8_t gp6=0, uint8_t gp7=0);
    virtual ~CounterAgent();

    virtual void on(uint8_t count);
    virtual void off();
    virtual void blink(uint8_t count);

protected:
    virtual void run();
    virtual configSTACK_DEPTH_TYPE getMaxStackSize();
    virtual void init();
    virtual void setLeds(uint8_t count);

    uint8_t pLedPads[COUNT_LEDS];
    QueueHandle_t xCmdQ;
};

#endif
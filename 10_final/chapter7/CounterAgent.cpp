#include "CounterAgent.h"
#include "stdio.h"

enum CounterAction {CounterOff, CounterOn, CounterBlink};

struct CounterCmd {
    CounterAction   action;
    uint8_t         count;
};

typedef struct CounterCmd CounterCmdT;

CounterAgent::CounterAgent(uint8_t gp1, uint8_t gp2, uint8_t gp3, uint8_t gp4,
                           uint8_t gp5, uint8_t gp6, uint8_t gp7) {
    pLedPads[0] = gp1;
    pLedPads[1] = gp2;
    pLedPads[2] = gp3;
    pLedPads[3] = gp4;
    pLedPads[4] = gp5;
    pLedPads[5] = gp6;
    pLedPads[6] = gp7;

    xCmdQ = xQueueCreate( COUNT_QUEUE_LEN, sizeof(CounterCmd));
    if (xCmdQ == NULL){
        printf("ERROR: Unable to create Queue\n");
    }
}

CounterAgent::~CounterAgent() {
    if (xCmdQ != NULL){
        vQueueDelete(xCmdQ);
    }
}

void CounterAgent::init(){
    for (int i = 0 ; i < COUNT_LEDS; i++){
        if (pLedPads[i] != 0) { // Sécurité si pad non défini
            gpio_init(pLedPads[i]);
            gpio_set_dir(pLedPads[i], GPIO_OUT);
            gpio_put(pLedPads[i], 0);
        }
    }
}

void CounterAgent::run(){
    BaseType_t res;
    printf("Count Started\n");
    init();

    CounterAction action = CounterOff;
    uint8_t count = 0;
    bool blinkOn = false;
    CounterCmdT cmd;
    bool change;

    if (xCmdQ == NULL) return;

    while (true) {
        res = xQueueReceive(xCmdQ, (void *)&cmd, COUNT_BLINK_DELAY);
        if (res == pdTRUE){
            action = cmd.action;
            count =  cmd.count;
            change = true;
        } else {
            change = false;
        }

        switch(action){
        case CounterOff:
            if (change) setLeds(0);
            break;
        case CounterOn:
            if (change) setLeds(count);
            break;
        case CounterBlink:
            blinkOn = !blinkOn;
            setLeds(blinkOn ? count : 0);
            break;
        }
    }
}

configSTACK_DEPTH_TYPE CounterAgent::getMaxStackSize(){
    return 150;
}

void CounterAgent::setLeds(uint8_t count){
    for (int i = 0; i < COUNT_LEDS; i ++){
        uint8_t m = 1 << i;
        gpio_put(pLedPads[i], (count & m) ? 1 : 0);
    }
}

void CounterAgent::on(uint8_t count){
    CounterCmdT cmd = {CounterOn, count};
    if (xCmdQ != NULL) xQueueSendToBack(xCmdQ, &cmd, 0);
}

void CounterAgent::off(){
    CounterCmdT cmd = {CounterOff, 0};
    if (xCmdQ != NULL) xQueueSendToBack(xCmdQ, &cmd, 0);
}

void CounterAgent::blink(uint8_t count){
    CounterCmdT cmd = {CounterBlink, count};
    if (xCmdQ != NULL) xQueueSendToBack(xCmdQ, &cmd, 0);
}
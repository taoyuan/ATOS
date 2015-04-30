#include "Timers.h"
#include "ATOS.h"
ATOS tos; //create a new istance

//variables to manage the LEDs
const byte LED1 = RED_LED;
const byte LED2 = GREEN_LED;
byte LED1status = 1;
byte LED2status = 1;
unsigned int testVar = 0;

//this task will flash the 1st LED
int flashLed1(void *params) {
    LED1status ^= 1;
    digitalWrite(LED1, LED1status);
    tos.sleep(500);
    return TASK_RET_INPROG;
}


//this task will flash the 2nd LED
int flashLed2(void *params) {
    LED2status ^= 1;
    digitalWrite(LED2, LED2status);
    tos.sleep(250);
    return TASK_RET_INPROG;
}

int printData(void *params) {
    Serial.println(testVar, DEC);
    testVar++;
    tos.sleep(1000);
    return TASK_RET_INPROG;
}

//setup routine
void setup() {
    tos.begin(); //initialize the scheduler
    //pins as output
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    //initialize the serial comm. and wait 2 secs. before to start the tasks
    Serial.begin(115200);
    delay(2000);
    //add the tasks
    tos.createTask(flashLed1);
    tos.createTask(flashLed2);
    tos.createTask(printData);
}


//main loop - it's empty
void loop() {}
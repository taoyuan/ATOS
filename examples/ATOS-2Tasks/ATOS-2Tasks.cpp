#include "Timers.h"
#include "ATOS.h"
ATOS atos; //create a new istance

//variables to manage the LEDs
const byte LED1 = RED_LED;
const byte LED2 = GREEN_LED;
byte LED1status = 1;
byte LED2status = 1;

//this task will flash the 1st LED
int flashLed1(void *params) {
    LED1status ^= 1;
    digitalWrite(LED1, LED1status);
    atos.sleep(1000);
}


//this task will flash the 2nd LED
int flashLed2(void *params) {
    LED2status ^= 1;
    digitalWrite(LED2, LED2status);
    atos.sleep(250);
}


//setup routine
void setup() {
    atos.begin(); //initialize the scheduler
    //pins as output
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    //add the tasks
    atos.createTask(flashLed1);
    atos.createTask(flashLed2);
}


//main loop - it's empty
void loop() {}
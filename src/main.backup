#include <Arduino.h>
#include <TaskScheduler.h>
#include "controller/Controller.h"

#define VRY_PIN 26
#define VRX_PIN 25
#define J_SW 27

Controller controller(VRX_PIN, VRY_PIN);

Scheduler runner;

Task t1(300, TASK_FOREVER, []() { controller.update(); });

void setup() {
    Serial.begin(9600);
    controller.init();

    runner.init();
    runner.addTask(t1);
    t1.enable(); 
}

void loop() {
    runner.execute();  
    int currentCommand = controller.getState();
    Serial.println(currentCommand);
}

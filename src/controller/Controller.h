#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

#define  LEFT_THRESHOLD  100
#define RIGHT_THRESHOLD 4000
#define UP_THRESHOLD    100
#define DOWN_THRESHOLD  4000

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

class Controller {
public:
    Controller(int pinX, int pinY);
    void init();
    void update();
    void get_state();
    int getState() const;  

private:
    int pinX, pinY;
    int valueX, valueY;
    int command;

    void printCommands();
};

#endif // CONTROLLER_H

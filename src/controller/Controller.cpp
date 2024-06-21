#include "Controller.h"

Controller::Controller(int pinX, int pinY)
    : pinX(pinX), pinY(pinY), command(COMMAND_NO) {}

void Controller::init() {
    //DO EXTEND BUTTONS I2C
}

int Controller::getState() const {
    return command;
}

void Controller::update() {
    valueX = analogRead(pinX);
    valueY = analogRead(pinY);
    command = COMMAND_NO;

    if (valueX < LEFT_THRESHOLD)
        command = command | COMMAND_LEFT;
    else if (valueX > RIGHT_THRESHOLD)
        command = command | COMMAND_RIGHT;

    if (valueY < UP_THRESHOLD)
        command = command | COMMAND_UP;
    else if (valueY > DOWN_THRESHOLD)
        command = command | COMMAND_DOWN;

    // printCommands();
}

void Controller::printCommands() {
    if (command & COMMAND_LEFT) {
        Serial.println("COMMAND LEFT");
    }
    if (command & COMMAND_RIGHT) {
        Serial.println("COMMAND RIGHT");
    }
    if (command & COMMAND_UP) {
        Serial.println("COMMAND UP");
    }
    if (command & COMMAND_DOWN) {
        Serial.println("COMMAND DOWN");
    }
}

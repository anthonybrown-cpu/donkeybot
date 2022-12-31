#pragma once

#include <stdint.h>

class Motor{
public:
    void setSpeed(int8_t speed);
    int8_t getSpeed();
    void stop();
private:
    char controlPin;
};
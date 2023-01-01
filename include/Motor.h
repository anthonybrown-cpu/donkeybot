#pragma once

#include <stdint.h>

class Motor
{
public:
    Motor(uint8_t pin, uint8_t pwmChannel);
    ~Motor();

    void setSpeed(float speed);
    float getSpeed();
    void stop();

private:
    char controlPin;
    char pwmChannel;
    float speed;
};
#pragma once

#include <stdint.h>

class Steering
{
public:
    Steering(uint8_t pin, uint8_t channel);
    void set(float value);
    float get();
    void center();

private:
    uint8_t controlPin;
    uint8_t pwmChannel;
    float steeringValue;
};
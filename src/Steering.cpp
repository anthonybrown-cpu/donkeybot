#include <Arduino.h>

#include "Steering.h"

#define PWM_RESOLUTION 8 // bits
#define PWM_FREQ 50 // Hz

#define FULL_RIGHT 13
#define CENTER 21
#define FULL_LEFT 27
#define STEERING_GAP (FULL_LEFT - FULL_RIGHT)

Steering::Steering(uint8_t pin, uint8_t channel){
    this->controlPin = pin;
    this->pwmChannel = channel;
    ledcSetup(channel, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(pin, channel);
    this->center();
}

void Steering::set(float value){
    this->steeringValue = value;
    ledcWrite(this->pwmChannel, FULL_RIGHT + (STEERING_GAP * value));
}

float Steering::get(){
    return this->steeringValue;
}

void Steering::center(){
    this->set(0.5);
}
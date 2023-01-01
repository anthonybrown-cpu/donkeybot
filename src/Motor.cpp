#include <Arduino.h>

#include "Motor.h"

#define PWM_RESOLUTION 8 // bits
#define PWM_FREQ 50 // Hz

#define FULL_REVERSE 12
#define IDLE 19
#define FULL_FORWARD 27
#define THROTTLE_GAP (FULL_FORWARD - FULL_REVERSE)

Motor::Motor(uint8_t pin, uint8_t channel)
{
    this->controlPin = pin;
    this->pwmChannel = channel;
    ledcSetup(channel, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(pin, channel);
    this->stop();
};

Motor::~Motor(){
    ledcDetachPin(this->controlPin);
}

void Motor::setSpeed(float speed)
{
    this->speed = speed;

    ledcWrite(this->pwmChannel, FULL_REVERSE + (THROTTLE_GAP * speed));
}

float Motor::getSpeed()
{
    return this->speed;
}

void Motor::stop(){
    this->setSpeed(0.5);
}
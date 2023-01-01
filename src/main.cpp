#include <Arduino.h>

#include "Configuration.h"
#include "Motor.h"
#include "Steering.h"

Motor motor(MOTOR_PIN, MOTOR_PWM_CHANNEL);
Steering steering(STEERING_PIN, STEERING_PWM_CHANNEL);

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  steering.set(0.5);
  sleep(1);
  steering.set(0);
  sleep(1);
  steering.set(0.5);
  sleep(1);
  steering.set(1);
  sleep(1);
}
#include <Arduino.h>
#include <LoRa.h>
#include <LiquidCrystal_I2C.h>

#include "Configuration.h"
#include "Board.h"
#include "Motor.h"
#include "Steering.h"

Motor motor(MOTOR_PIN, MOTOR_PWM_CHANNEL);
Steering steering(STEERING_PIN, STEERING_PWM_CHANNEL);

//LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void emergencyStop();

void msgHandler(String msg){
  size_t thr = msg.indexOf("THR") + 4;
  size_t str = msg.indexOf("STR") + 4;

  float throttle_val;
  if(msg.indexOf("THR") != -1){
    throttle_val = msg.substring(thr, msg.indexOf(" ", thr)).toFloat();
    motor.setSpeed(throttle_val);
  }

  float steering_val;
  if(msg.indexOf("STR") != -1){
    steering_val = msg.substring(str, msg.indexOf(" ", str)).toFloat();
    steering.set(steering_val);
  }

  
  

  //lcd.clear();
  //lcd.print("THR: ");
  //lcd.print(throttle_val);

  //lcd.setCursor(0, 1);
  //lcd.print("STR: ");
  //lcd.print(steering_val);
  
}

void setup() {
  initBoard();

  //lcd.init();
  //lcd.backlight();

  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(LORA_FREQUENCY)) {
      Serial.println("Starting LoRa failed!");
      //lcd.print("LoRa init fail");
      while (1);
  }

  LoRa.setTxPower(15);
  LoRa.setSyncWord(0x12);

  //lcd.print("Ready!");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if(packetSize){
    String recv;
    for(int i = 0; i < packetSize; i++) {
        recv += (char)LoRa.read();
    }
    recv.trim();
    msgHandler(recv);
  }
}

void emergencyStop(){
  steering.center();
  motor.stop();
}
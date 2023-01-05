#include <Arduino.h>
#include <LoRa.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "Configuration.h"
#include "Board.h"

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  initBoard();
  delay(1500);

  lcd.init();
  lcd.backlight();
  lcd.print("LoRa Message");
  lcd.setCursor(0, 1);
  lcd.print("Receiver!");

  Serial.println("LoRa-LCD");

  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(LORA_FREQUENCY)) {
      Serial.println("Starting LoRa failed!");
      while (1);
  }
  LoRa.setTxPower(15);

  Serial.println("Init complete!");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if(packetSize){
    String recv = "";
    // read packet
    while (LoRa.available()) {
        recv += (char)LoRa.read();
    }
    recv.trim();

    Serial.print("Got a LoRa message: '");
    Serial.println(recv);
    Serial.println("'");
  
    lcd.clear();
    lcd.print(recv);

    // Send the recv message back to the sender
    LoRa.beginPacket();
    LoRa.println(recv);
    LoRa.endPacket();

  }
}
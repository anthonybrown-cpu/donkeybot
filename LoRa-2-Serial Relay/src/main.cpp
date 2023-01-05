#include <Arduino.h>
#include <LoRa.h>
#include "Board.h"

void sendLoRaMessage(String msg);

void setup() {
  initBoard();
  delay(1500);

  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  Serial.println("LoRa pins set");
  if (!LoRa.begin(LORA_FREQUENCY)) {
      Serial.println("Starting LoRa failed!");
      while (1);
  }
  Serial.println("LoRa init complete!");
  LoRa.setTxPower(15);

  Serial.println("Init complete!");
}

void loop() {
  if(Serial.available() > 0){
    Serial.println("GOT SERIAL MESSAGE!");
    String message = Serial.readStringUntil('\n');

    sendLoRaMessage(message);
    Serial.print("Sending LoRa message: ");
    Serial.println(message);
  }

  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String recv = "";
    // read packet
    while (LoRa.available()) {
        recv += (char)LoRa.read();
    }
    recv.trim(); // Remove newline

    Serial.print("Got LoRa MSG: '");
    Serial.print(recv);

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

void sendLoRaMessage(String msg){
  LoRa.beginPacket();
  LoRa.println(msg);
  LoRa.endPacket();
}

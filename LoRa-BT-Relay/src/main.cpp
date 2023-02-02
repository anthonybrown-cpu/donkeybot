#include <Arduino.h>
#include <BluetoothSerial.h>
#include <LoRa.h>
#include "Board.h"

BluetoothSerial SerialBT;

void sendLoRaMessage(String msg);
void processLoRaMessage();
void processBluetoothMessage();

void setup() {
  initBoard();
  delay(1500);

  // BLUETOOTH
  SerialBT.begin("ESP32_RELAY"); //Bluetooth device name
  Serial.println("BT Init Complete");
  
  // LORA
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(LORA_FREQUENCY)) {
      Serial.println("Starting LoRa failed!");
      while (1);
  }
  Serial.println("LoRa Init Complete");
  LoRa.setTxPower(15);
  LoRa.setSyncWord(0x12);

  Serial.println("App Init Complete");
}

void loop() {
  // BT -> LoRa
  if (SerialBT.available()) {
    processBluetoothMessage();
  }

  // LoRa -> BT
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    processLoRaMessage();
  }

  delay(20);
}

void processBluetoothMessage(){
  String msg = "";
  while(SerialBT.available()){
    msg += (char)SerialBT.read();
  }
  //String msg = SerialBT.readStringUntil('\n');
  Serial.print("Received BT: ");
  Serial.println(msg);

  //sendLoRaMessage(message);
}

void processLoRaMessage(){
  String msg = "";
  // read packet
  while (LoRa.available()) {
      msg += (char)LoRa.read();
  }
  msg.trim(); // Remove newline

  Serial.print("Got LoRa MSG: '");
  Serial.print(msg);

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());

  SerialBT.print(msg);
}

void sendLoRaMessage(String msg){
  LoRa.beginPacket();
  LoRa.println(msg);
  LoRa.endPacket();
}

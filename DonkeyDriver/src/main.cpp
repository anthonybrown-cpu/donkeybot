#include <Arduino.h>
#include <WiFi.h>

#include "Configuration.h"
#include "Motor.h"
#include "Steering.h"

#define WIFI_SSID "TAP lab"
#define WIFI_PASSWORD "Hack&Snack"

#define WIFI_LED 33
#define READY_LED 32

WiFiServer server(80);

Motor motor(MOTOR_PIN, MOTOR_PWM_CHANNEL);
Steering steering(STEERING_PIN, STEERING_PWM_CHANNEL);

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void initWiFi();
void emergencyStop();

void clientHandler(WiFiClient* client){
  String msg;
  while(client->available()){
    msg += char(client->read());
  }

  size_t thr = msg.indexOf("THR") + 4;
  size_t str = msg.indexOf("STR") + 4;

  float throttle_val = msg.substring(thr, msg.indexOf(" ", thr)).toFloat();
  float steering_val = msg.substring(str, msg.indexOf(" ", str)).toFloat();

  motor.setSpeed(throttle_val);
  steering.set(steering_val);
  
}

void setup() {
  pinMode(READY_LED, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);

  digitalWrite(READY_LED, LOW);
  digitalWrite(WIFI_LED, LOW);

  Serial.begin(9600);
  initWiFi();

  server.begin();

  digitalWrite(READY_LED, HIGH);
}

void loop() {
  WiFiClient client = server.available();
  if (client){
    if (client.connected()){
      //Serial.println("Connected to client!");
      if(client.available()){
        clientHandler(&client);
      }
    }

    client.stop();
  }
}

void initWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(!WiFi.isConnected()){
    WiFi.reconnect();
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi Failed!");
    }
  }
  digitalWrite(WIFI_LED, HIGH);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void emergencyStop(){
  steering.center();
  motor.stop();
}
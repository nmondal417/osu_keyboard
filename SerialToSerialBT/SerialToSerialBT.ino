//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial
#include <string.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

long batt_timer;
int batt_voltage = A13;

String message = "";
char incomingChar;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32 Keyboard"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  batt_timer = millis();
  pinMode(batt_voltage, INPUT);
}

void loop() {
  if (millis() - batt_timer > 1000) {
      SerialBT.println(analogRead(batt_voltage)*3.3/4096*2);
      Serial.println("Hello world");
      Serial.println(SerialBT.available());
      batt_timer = millis();
  }  

  if (SerialBT.available()){
    if (millis() - batt_timer > 1000) {
      SerialBT.println(analogRead(batt_voltage)*3.3/4096*2);
      Serial.println("Hello world");
      Serial.println(SerialBT.available());
      batt_timer = millis();
    }  
  }
}

/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

int key_f = 12;
int key_j = 27;
int led = 13;
long led_time;
//int f = 102;
//int j= 106;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();

  pinMode(key_f, INPUT);
  pinMode(key_j, INPUT);
  pinMode(led, OUTPUT);
  led_time = millis();
  digitalWrite(led, LOW);
}

void loop() {

  if (millis()-led_time > 1000) {
    digitalWrite(led, HIGH);
  }
  if (millis()-led_time > 2000) {
    digitalWrite(led, LOW);
    led_time = millis();
  }
  if(bleKeyboard.isConnected()) {
    Serial.println("connected");
    
    if (digitalRead(key_f) == HIGH) {
      Serial.println("F pressed");
      bleKeyboard.press('f');
    }
    else {
      //bleKeyboard.release('f');
    }

    if (digitalRead(key_j) == HIGH) {
      Serial.println("J pressed");
      bleKeyboard.press('j');
    }
    else {
      //bleKeyboard.release('j');
    }
    bleKeyboard.releaseAll();

  }
  
  Serial.println("finished");
  delay(1);

}

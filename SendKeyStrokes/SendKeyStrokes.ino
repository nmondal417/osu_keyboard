/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

int key_f_state = 0;
int prev_key_f_state = 0;
int key_j_state = 0;
int prev_key_j_state = 0;
int key_f = 36;
int key_j = 39;
int led = 13;
long led_time;

bool prev_ble_state;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  //bleKeyboard.setDelay(10);
  pinMode(key_f, INPUT);
  pinMode(key_j, INPUT);
  
  pinMode(led, OUTPUT);
  led_time = millis();
  digitalWrite(led, LOW);

  prev_ble_state = bleKeyboard.isConnected();
  delay(100);
}

void loop() {

  if (millis()-led_time > 1000) {
    digitalWrite(led, HIGH);
  }
  if (millis()-led_time > 2000) {
    digitalWrite(led, LOW);
    led_time = millis();
  }

  if (bleKeyboard.isConnected() && !prev_ble_state) {
    Serial.println("here");
    delay(10);
  }
  
  if(bleKeyboard.isConnected()) {

    key_f_state = digitalRead(key_f);
    key_j_state = digitalRead(key_j);
    //Serial.println("connected");
    //Serial.println("Sending 'Hello world'...");
    //bleKeyboard.print("Hello world");

    //delay(1000);

    //Serial.println("Sending Enter key...");
    //bleKeyboard.write(KEY_RETURN);

    //delay(1000);

    //Serial.println("Sending Play/Pause media key...");
    //bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

    //delay(1000);

    //Serial.println("Sending Ctrl+Alt+Delete...");
    //Serial.println("garbage");
    //Serial.println("more garbage");
    //Serial.println("yeet");
    //Serial.println("skrrt");
    //Serial.println("hewwo");
    if (key_f_state && !prev_key_f_state) {
      bleKeyboard.press('f');
      Serial.println("press f");
    }
    else if (!key_f_state && prev_key_f_state) {
      bleKeyboard.release('f');
      Serial.println("release f");
    }
    /*
    //delay(100);
    else{
      bleKeyboard.release('f');
    } */

    if (key_j_state && !prev_key_j_state) {
      bleKeyboard.press('j');
      Serial.println("press j");
    }
    else if (!key_j_state && prev_key_j_state) {
      bleKeyboard.release('j');
      Serial.println("release j");
    }

    prev_key_f_state = key_f_state;
    prev_key_j_state = key_j_state;
    
    /*
    //delay(100);
    else{
      bleKeyboard.release('j');
    }  
    */
  }
  //Serial.println("done");
  //Serial.println("Waiting 5 seconds...");
  prev_ble_state = bleKeyboard.isConnected(); 
}

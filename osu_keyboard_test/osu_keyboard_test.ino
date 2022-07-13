#if ARDUINO_USB_MODE
void setup(){}
void loop(){}
#else


#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;

const int left = 36;
const int right = 39;
/*
const int RELEASE = 0;
const int PRESS = 1;
const int HELD = 2;
int state;
long press_timer;
long held_timer;*/

void setup() {
  // put your setup code here, to run once:
  pinMode(left, INPUT);
  pinMode(right, INPUT);
  Keyboard.begin();
  //state = RELEASE;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!digitalRead(left)) {
    Keyboard.press('f');
    Serial.println("f");
    //state = PRESS;
  }
  else {
    Keyboard.release('f');
  }
  
  if (!digitalRead(right)) {
    Keyboard.press('j');
    Serial.println("j");

  }
  else {
    Keyboard.release('j');
  }
}

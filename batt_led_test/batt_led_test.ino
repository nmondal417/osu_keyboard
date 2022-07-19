long timer;

void setup() { 
  Serial.begin(115200);
  delay(100);
  timer = millis();
}

void loop() {
  if (millis() - timer > 1000) {
    Serial.println(analogRead(A0));
    timer = millis();
  }
}

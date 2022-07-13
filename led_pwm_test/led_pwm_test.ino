
int rgb [3] = {16, 17, 21};

int rgb_cycler;

int pwm = 0;

void setup() {
  Serial.begin(115200);
  ledcAttachPin(rgb[0], 0);
  ledcAttachPin(rgb[1], 1);
  ledcAttachPin(rgb[2], 2);
  ledcSetup(0, 1000, 8);
  ledcSetup(1, 1000, 8);
  ledcSetup(2, 1000, 8);

  ledcWrite(0, 255); //start with red
}

void loop() {
  
  pwm = 0;
  while(pwm < 255) {   //yellow
    ledcWrite(1, pwm++);
    delay(10);
  }
  delay(1000);

  pwm = 255;
  while(pwm > 0) {   //green
    ledcWrite(0, pwm--);
    delay(10);
  }
  delay(1000);

  pwm = 0;
  while(pwm < 255) {  //cyan??
    ledcWrite(2, pwm++);
    delay(10);
  }
  delay(1000);

  pwm = 255;
  while(pwm > 0) { //blue
    ledcWrite(1, pwm--);
    delay(10);
  }
  delay(1000);

  pwm = 0;
  while(pwm < 255) { //violet
    ledcWrite(0, pwm++);
    delay(10);
  }
  delay(1000);

  pwm = 255;
  while(pwm > 0) { //red
    ledcWrite(2, pwm--);
    delay(10);
  }
  delay(1000);
  

}

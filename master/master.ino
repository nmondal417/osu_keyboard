#include <string.h>

#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

//ESP LED channels
const int R [4] = {0, 3, 6, 9};
const int G [4] = {1, 4, 7, 10};
const int B [4] = {2, 5, 8, 11};


bool rgb_mode = 1;    //0 for LED color sweep, 1 for LED trigger on keypress

int rgb_pins [4][3] = {{16, 17, 21}, {5, 18, 19}, {14, 22, 23}, {33, 15, 32}};
int rgb_pwm [4][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};   //pwm values for LEDs (0-255)
int off [3] = {0, 0, 0};   //pwm value for off LED

int curr_state [4][3];    //pwm state the LEDs are transitioning FROM
int curr_state_idx;       
int next_state [4][3];    //pwm state the LEDs are transitioning TO
int next_state_idx;       

int fade_delay = 500;  //time between pwm writes, in microseconds
long fade_time_marker;   

//bool exec_fade = 1;  //set to 0 to not run fade

//for mode 0///
int states [6][3] = {{255, 0, 0},     //LED pwm states that will be cycled
                     {128, 128, 0},
                     {0, 255, 0},
                     {0, 128, 128},
                     {0, 0, 255},
                     {128, 0, 128}};
                     
int state_delay = 2000;   //time between state cycling, in milliseconds
long state_time_marker;


//for mode 1///
int trigger_colors [4][3] = {{255, 0, 0}, {0, 0, 255}, {0, 255, 0}, {0, 161, 222}};   //colors that LEDs will light up when associated key is pressed

/////////////////////////////////////////

int keys[4] = {36, 39, 34, 25};
char key_letters [4] = {'d', 'f', 'j', 'k'};
int key_states[4] = {0, 0, 0, 0};
int prev_key_states[4] = {0, 0, 0, 0};

int led = 13;
long led_time;

bool prev_ble_state;


void copy(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}


void fade() {
  if (micros() - fade_time_marker > fade_delay) {
    for (int j = 0; j < sizeof(rgb_pins)/sizeof(rgb_pins[0]); j++) {   //for each LED
      for (int i = 0; i < 3; i++) {    //for each of R, G, and B
        if (rgb_pwm[j][i] < next_state[j][i]) {
          rgb_pwm[j][i] += 1;
          ledcWrite(3*j+i, rgb_pwm[j][i]);
          fade_time_marker = micros();
        }
        else if (rgb_pwm[j][i] > next_state[j][i]) {
          rgb_pwm[j][i] -= 1;
          ledcWrite(3*j+i, rgb_pwm[j][i]);
          fade_time_marker = micros();
        }
        else {
          ledcWrite(3*j+i, rgb_pwm[j][i]);
        }
      }
      
    }
  }
}

void setup() {
  for (int i = 0; i < sizeof(rgb_pins)/sizeof(rgb_pins[0]); i++) { //setup LED PWM channels
    ledcAttachPin(rgb_pins[i][0], R[i]);
    ledcAttachPin(rgb_pins[i][1], G[i]);
    ledcAttachPin(rgb_pins[i][2], B[i]);
    ledcSetup(R[i], 1000, 8);
    ledcSetup(G[i], 1000, 8);
    ledcSetup(B[i], 1000, 8);
  }

  if (!rgb_mode) {
    state_time_marker = millis();
  
  
    curr_state_idx = 0;
    next_state_idx = 0;
    delay(state_delay);
    delay(1000);

    for (int i = 0; i < sizeof(rgb_pins)/sizeof(rgb_pins[0]); i++) {
      copy(states[curr_state_idx], rgb_pwm[i], 3);   //set PWM values to first color on the list
    }

  }
  
  
    


  ///////////////////////////////////////
  
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  //bleKeyboard.setDelay(8);
  for (int key = 0; key < sizeof(keys)/sizeof(keys[0]); key++){
    pinMode(keys[key], INPUT);
  }
  
  pinMode(led, OUTPUT);
  led_time = millis();
  digitalWrite(led, LOW);

  prev_ble_state = bleKeyboard.isConnected();
  delay(100);
}

void loop() {

  if (!rgb_mode) {
    if (millis() - state_time_marker > state_delay) {
      curr_state_idx = next_state_idx;      //update current state
      for (int i = 0; i < sizeof(rgb_pins)/sizeof(rgb_pins[0]); i++) {
        copy(states[curr_state_idx], curr_state[i], 3);    //set all LEDs to the same current state
      }
      //copy(states[curr_state_idx], curr_state, 3);
      //curr_state = states[curr_state_idx];
      if (curr_state_idx + 1 < sizeof(states)/sizeof(states[0])) {
        next_state_idx = curr_state_idx + 1;
      }
      else {
        next_state_idx = 0;  //wrap around if we are at the end of the states array
      }
      for (int i = 0; i < sizeof(rgb_pins)/sizeof(rgb_pins[0]); i++) {
        copy(states[curr_state_idx], rgb_pwm[i], 3);
        copy(states[next_state_idx], next_state[i], 3);
      }
      //copy(states[curr_state_idx], rgb_pwm, 3);
      //copy(states[next_state_idx], next_state, 3);
      state_time_marker = millis();
      /*
      Serial.println("update");
      Serial.println(curr_state_idx);
      Serial.println(next_state_idx);
      Serial.print("current state: ");
      Serial.print(String(curr_state[0]));
      Serial.print("  ");
      Serial.print(String(curr_state[1]));
      Serial.print("  ");
      Serial.println(String(curr_state[2]));
      
      Serial.print("next state: ");
      Serial.print(String(next_state[0]));
      Serial.print("  ");
      Serial.print(String(next_state[1]));
      Serial.print("  ");
      Serial.println(String(next_state[2]));
  
      Serial.println(); */
    }
  

  }
  ///////////////////////////////////////////////
   //blink to onboard LED to check that the ESP32 is powered on
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


    for (int key = 0; key < sizeof(keys)/sizeof(keys[0]); key++){
      key_states[key] = digitalRead(keys[key]);

      int key_state = key_states[key];
      int prev_key_state = prev_key_states[key];
      char letter = key_letters[key];

      if (key_state && !prev_key_state) {
      bleKeyboard.press(letter);
      Serial.print(letter);
      Serial.println(" pressed");

        if (rgb_mode) {
          copy(trigger_colors[key], rgb_pwm[key], 3);
          copy(trigger_colors[key], next_state[key], 3);
        }
      }
      else if (!key_state && prev_key_state) {   
        bleKeyboard.release(letter);
        Serial.print(letter);
        Serial.println(" released");


        if (rgb_mode) {
          copy(off, next_state[key], 3);
  
        }

        Serial.print("curent color: ");
        Serial.print(String(rgb_pwm[2][0]));
        Serial.print("  ");
        Serial.print(String(rgb_pwm[2][1]));
        Serial.print("  ");
        Serial.println(String(rgb_pwm[2][2]));
      
      }
      prev_key_states[key] = key_states[key];
 
    }
  }

  fade();
  
  prev_ble_state = bleKeyboard.isConnected(); 

    /*
    key_f_state = digitalRead(key_f);
    key_j_state = digitalRead(key_j);
    
    if (key_f_state && !prev_key_f_state) {
      bleKeyboard.press('f');
      Serial.println("press f");

      if (rgb_mode) {
        copy(trigger_colors[0], rgb_pwm[0], 3);
        copy(trigger_colors[0], next_state[0], 3);
      }

      

      Serial.print("curent color: ");
      Serial.print(String(rgb_pwm[0][0]));
      Serial.print("  ");
      Serial.print(String(rgb_pwm[0][1]));
      Serial.print("  ");
      Serial.println(String(rgb_pwm[0][2]));

      Serial.print("next state: ");
      Serial.print(String(next_state[0][0]));
      Serial.print("  ");
      Serial.print(String(next_state[0][1]));
      Serial.print("  ");
      Serial.println(String(next_state[0][2]));  
    }
    else if (!key_f_state && prev_key_f_state) {   
      bleKeyboard.release('f');

      if (rgb_mode) {
        copy(off, next_state[0], 3);

      }
      
      Serial.println("release f");

      Serial.print("curent color: ");
      Serial.print(String(rgb_pwm[0][0]));
      Serial.print("  ");
      Serial.print(String(rgb_pwm[0][1]));
      Serial.print("  ");
      Serial.println(String(rgb_pwm[0][2]));

      Serial.print("next state: ");
      Serial.print(String(next_state[0][0]));
      Serial.print("  ");
      Serial.print(String(next_state[0][1]));
      Serial.print("  ");
      Serial.println(String(next_state[0][2])); 
    }

    if (key_j_state && !prev_key_j_state) {
      bleKeyboard.press('j');
      Serial.println("press j");

      if (rgb_mode) {
        copy(trigger_colors[1], rgb_pwm[1], 3);
        copy(trigger_colors[1], next_state[1], 3);
      }

      Serial.print("curent color: ");
      Serial.print(String(rgb_pwm[1][0]));
      Serial.print("  ");
      Serial.print(String(rgb_pwm[1][1]));
      Serial.print("  ");
      Serial.println(String(rgb_pwm[1][2]));

      Serial.print("next state: ");
      Serial.print(String(next_state[1][0]));
      Serial.print("  ");
      Serial.print(String(next_state[1][1]));
      Serial.print("  ");
      Serial.println(String(next_state[1][2])); 
      
    }
    else if (!key_j_state && prev_key_j_state) {
      bleKeyboard.release('j');
      Serial.println("release j");

      if (rgb_mode) {
        copy(off, next_state[1], 3);
      }

      Serial.print("curent color: ");
      Serial.print(String(rgb_pwm[1][0]));
      Serial.print("  ");
      Serial.print(String(rgb_pwm[1][1]));
      Serial.print("  ");
      Serial.println(String(rgb_pwm[1][2]));

      Serial.print("next state: ");
      Serial.print(String(next_state[1][0]));
      Serial.print("  ");
      Serial.print(String(next_state[1][1]));
      Serial.print("  ");
      Serial.println(String(next_state[1][2])); 
    }

    prev_key_f_state = key_f_state;
    prev_key_j_state = key_j_state;
    
  }   */
  

}

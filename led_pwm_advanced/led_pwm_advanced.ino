#include <string.h>

const int R = 0;
const int G = 1;
const int B = 2;
/*
const int off [3] = {0, 0, 0};
const int red [3] {255, 0, 0};
const int yellow [3] = {255, 255, 0};
const int green [3] = {0, 225, 0};
const int cyan [3] = {0, 255, 255};
const int blue [3] = {0, 0, 255};
const int magneta [3] = {255, 0, 255};
const int white [3] = {128, 128, 128};*/
/*
const int CSTATE0 [3] = {255, 0, 0};
const int CSTATE1 [3] = {0, 255, 0};
const int CSTATE2 [3] = {0, 0, 255}; */
int states [3][3] = {{255, 0, 0},
                     {255, 0, 0},
                     {50, 0, 0}};//,
                     //{0, 255, 0},
                     //{0, 128, 128},
                     //{0, 0, 255},
                     //{128, 0, 128}};

int state_delay = 2000;
int fade_delay = 5;



int rgb_pins [3] = {5, 18, 19};
int rgb_pwm [3] = {0, 0, 0};

long state_time_marker;
long fade_time_marker;

int curr_state [3];
int curr_state_idx;
int next_state [3];
int next_state_idx;

void copy(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

void setup() {
  Serial.begin(115200);
  ledcAttachPin(rgb_pins[R], R);
  ledcAttachPin(rgb_pins[G], G);
  ledcAttachPin(rgb_pins[B], B);
  ledcSetup(R, 1000, 8);
  ledcSetup(G, 1000, 8);
  ledcSetup(B, 1000, 8);

  state_time_marker = millis();


  curr_state_idx = 0;
  next_state_idx = 0;
  delay(state_delay);
  delay(1000);

  copy(states[curr_state_idx], rgb_pwm, 3);
  //rgb_pwm = states[curr_state_idx];
   
}


void fade() {
  for (int i = 0; i < 3; i++) {
    if (rgb_pwm[i] < next_state[i] && millis() - fade_time_marker > fade_delay) {
      rgb_pwm[i] += 1;
      ledcWrite(i, rgb_pwm[i]);
      fade_time_marker = millis();
    }
    else if (rgb_pwm[i] > next_state[i] && millis() - fade_time_marker > fade_delay) {
      rgb_pwm[i] -= 1;
      ledcWrite(i, rgb_pwm[i]);
      fade_time_marker = millis();
    }
  }
  
}

void loop() {

  if (millis() - state_time_marker > state_delay) {
    curr_state_idx = next_state_idx;
    copy(states[curr_state_idx], curr_state, 3);
    //curr_state = states[curr_state_idx];
    if (curr_state_idx + 1 < sizeof(states)/sizeof(states[0])) {
      next_state_idx = curr_state_idx + 1;
    }
    else {
      next_state_idx = 0;
    }
    copy(states[curr_state_idx], rgb_pwm, 3);
    copy(states[next_state_idx], next_state, 3);
    state_time_marker = millis();
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

    Serial.println();
  }

  fade();

}

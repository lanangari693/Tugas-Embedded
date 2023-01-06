#include <Arduino.h>
#include "fsm.h"
bool tombol_B = false;
bool tombol_T = false;
int state = 0;
int state_move = state_accelerate;

double kecepatan = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(5,INPUT);
  pinMode(2,INPUT);
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  tombol_B = digitalRead(5);
  tombol_T = digitalRead(2);
  // Serial.print(state);
  // Serial.print("  ");
  // Serial.print(tombol_B);
  // Serial.print(tombol_T);
  // Serial.print("  ");
  // Serial.print(state_move);
  // Serial.print("  ");
  Serial.println(kecepatan);
  fsm(&state, &state_move, tombol_B, tombol_T, &kecepatan);
  delay(10);
}
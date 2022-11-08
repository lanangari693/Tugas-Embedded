#include <Arduino.h>
#include "fsm2.h"
#define out_led 2

bool in_but = 0;
int state = state_0;

void setup() {
  // put your setup code here, to run once:
  pinMode(out_led, OUTPUT);
  pinMode(in_but, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  in_but = digitalRead(5);
  digitalWrite(out_led, fsm(&state,in_but));
  Serial.println(state);
}
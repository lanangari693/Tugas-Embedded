#include <Arduino.h>
#include <iostream>

#define STATE1 1
#define STATE2 2
#define STATE3 3
#define STATE4 4

#define EVENT_A 11
#define EVENT_B 12

#define LED1 32
#define LED2 33
#define LED3 25
#define LED4 26
#define A 2
#define B 5

int state = STATE1;
int sinyalA = 0, sinyalB = 0;
int tombolA = 0, tombolB = 0;
int last_tombolA = 0, last_tombolB = 0;
int last_sinyalA = 0, last_sinyalB = 0;

hw_timer_t *My_timer = NULL;

int input_test_value = 0;
int last_input = 0; 
int output = 0;
unsigned long timeA = 0, last_timeA = 0, timeB = 0, last_timeB = 0;

void IRAM_ATTR change_tombol(){
  tombolA = digitalRead(A);
  tombolB = digitalRead(B);
}

void debounce_and_edgeDetectionA(int input){
  timeA = millis();
  if(input == 1 && last_tombolA == 0 && timeA-last_timeA > 250){ //rising detection
     sinyalA = !sinyalA;
     last_timeA = timeA;
  }
  last_tombolA = input;
}

void debounce_and_edgeDetectionB(int input){
  timeB = millis();
  if(input == 1 && last_tombolB == 0 && timeB-last_timeB > 250){ //rising detection
     sinyalB = !sinyalB;
     last_timeB = timeB;
  }
  last_tombolB = input;
}

void fsm(int sinA, int sinB){
  switch (state){
    case STATE1: {
        digitalWrite(LED1,HIGH);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        if(sinA != last_sinyalA){
          state = STATE2;
          last_sinyalA = sinyalA;
        }
        if(sinB != last_sinyalB){
          state = STATE4;
          last_sinyalB = sinyalB;
        }
        break;
      }
    case STATE2: {
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,HIGH);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,LOW);
        if(sinA != last_sinyalA){
          state = STATE3;
          last_sinyalA = sinyalA;
        }
        if(sinB != last_sinyalB){
          state = STATE1;
          last_sinyalB = sinyalB;
        }
        break;
      }
    case STATE3: {
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,HIGH);
        digitalWrite(LED4,LOW);
        if(sinA != last_sinyalA){
          state = STATE4;
          last_sinyalA = sinyalA;
        }
        if(sinB != last_sinyalB){
          state = STATE2;
          last_sinyalB = sinyalB;
        }
        break;
      }
    case STATE4: {
        digitalWrite(LED1,LOW);
        digitalWrite(LED2,LOW);
        digitalWrite(LED3,LOW);
        digitalWrite(LED4,HIGH);
        if(sinA != last_sinyalA){
          state = STATE1;
          last_sinyalA = sinyalA;
        }
        if(sinB != last_sinyalB){
          state = STATE3;
          last_sinyalB = sinyalB;
        }
        break;
      }
    default: {
        break;
      }
  }
}

void setup(){
  My_timer = timerBegin(0, 80, true); //Prescaler 80, base clock 80 MHz
  timerAttachInterrupt(My_timer, &change_tombol, true);
  timerAlarmWrite(My_timer, 100, true);
  timerAlarmEnable(My_timer);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}
void loop(){
  debounce_and_edgeDetectionA(tombolA);
  debounce_and_edgeDetectionB(tombolB);
  fsm(sinyalA, sinyalB);
}



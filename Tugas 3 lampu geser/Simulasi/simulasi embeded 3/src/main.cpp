#include <Arduino.h>
#include <iostream>

#define STATE1 1
#define STATE2 2
#define STATE3 3
#define STATE4 4

#define EVENT_A 11
#define EVENT_B 12

int state = STATE1;
int sinyalA = 0, sinyalB = 0;
int tombolA = 0, tombolB = 0;
int last_tombolA = 0, last_tombolB = 0;
int last_sinyalA = 0, last_sinyalB = 0;

hw_timer_t *My_timerA = NULL;
hw_timer_t *My_timerB = NULL;

int input_test_value = 0;
int last_input = 0; 
int output = 0;
unsigned long timeA = 0, last_timeA = 0, timeB = 0, last_timeB = 0;

void IRAM_ATTR change_tombolA(){
  tombolA = !tombolA;
}
void IRAM_ATTR change_tombolB(){
  tombolB = !tombolB;
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
  My_timerA = timerBegin(0, 80, true); //Prescaler 80, base clock 80 MHz
  timerAttachInterrupt(My_timerA, &change_tombolA, true);
  My_timerB = timerBegin(1, 80, true); //Prescaler 80, base clock 80 MHz
  timerAttachInterrupt(My_timerB, &change_tombolB, true);
  //Perubahan input cepat
  //timerAlarmWrite(My_timerA, 100, true);
  //timerAlarmWrite(My_timerB, 100, true);
  // Perubahan input lambat
  timerAlarmWrite(My_timerA, 50000, true);
  timerAlarmWrite(My_timerB, 70000, true);
  timerAlarmEnable(My_timerA);
  timerAlarmEnable(My_timerB);
  Serial.begin(9600);
}
void loop(){
  debounce_and_edgeDetectionA(tombolA);
  debounce_and_edgeDetectionB(tombolB);
  fsm(sinyalA, sinyalB);
  Serial.print(tombolA+11);
  Serial.print(",");
  Serial.print(tombolB+9);
  Serial.print(",");
  Serial.print(sinyalA+7);
  Serial.print(",");
  Serial.print(sinyalB+5);
  Serial.print(",");
  Serial.println(state);
}
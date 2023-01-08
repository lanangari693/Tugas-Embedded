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
int last_sinyalA = 0, last_sinyalB = 0;

void fsm(int sinA, int sinB){
  switch (state){
    case STATE1: {
        if(sinA != last_sinyalA){
          state = STATE2;
          last_sinyalA = sinyalA;
        }
        else if(sinB != last_sinyalB){
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
        else if(sinB != last_sinyalB){
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
        else if(sinB != last_sinyalB){
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
        else if(sinB != last_sinyalB){
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
  Serial.begin(9600);
}
void loop(){
  for(int i=0;i<100;i++){
    if(i%5==0){
      sinyalA = !sinyalA;
    }
    if(i%7==0){
      sinyalB = !sinyalB;
    }
    fsm(sinyalA, sinyalB);
    std::cout<<sinyalA<<" "<<sinyalB<<" "<<state<<"\n";
  }
}



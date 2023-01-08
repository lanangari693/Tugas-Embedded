#include <Arduino.h>
hw_timer_t *My_timer = NULL;

int input_test_value = 0;
int last_input = 0; 
int output = 0;
unsigned long time1 = 0, last_time = 0;

void IRAM_ATTR inputTest(){
  input_test_value = !input_test_value;
}

void debounce_and_edgeDetection(int input){
  time1 = millis();
  if(input == 1 && last_input == 0 && time1-last_time > 250){ //rising detection
     output = !output;
     last_time = time1;
  }
  last_input = input;
}

void setup() {
  My_timer = timerBegin(0, 80, true); //Prescaler 80, base clock 80 MHz
  timerAttachInterrupt(My_timer, &inputTest, true);
  //Perubahan input cepat
  //timerAlarmWrite(My_timer, 100, true);
  // Perubahan input lambat
  timerAlarmWrite(My_timer, 10000, true);
  timerAlarmEnable(My_timer);
  Serial.begin(9600);
}
void loop() {
  Serial.print(input_test_value);
  Serial.print(",");
  Serial.println(output);
  debounce_and_edgeDetection(input_test_value);
}
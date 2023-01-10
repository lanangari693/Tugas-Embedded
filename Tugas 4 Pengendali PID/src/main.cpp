#include <Arduino.h>
#include <TM1637Display.h>
#include <Preferences.h>

#define potensio 13
#define set 25
#define increment 26
#define decrement 27
#define CLK 5
#define DIO 18

#define state_ready 1
#define state_confKp 2
#define state_confKi 3
#define state_confKd 4

Preferences preferences;

int first_run = 1;
double inc = 0.01;
double avr_target_kecepatan = 0;
unsigned long time_cnt = 0, last_time_cnt = 0, time_hold = 0;
double Kp = 0, Ki = 0, Kd = 0, value = 0;
int state = state_ready;
double error_1 = 0, error_0 = 0, cumError = 0, rateError = 0;
double dt = 0.01;
double kecepatan = 0;
double out_plant = 0, out_plant_1 = 0, in_plant_1 = 0;
int target_kecepatan[10]={0};
int i = 0;
int jumlah = 0;

TM1637Display display = TM1637Display(CLK, DIO);
const uint8_t ready[] = {
  SEG_A | SEG_E | SEG_F, 0,0,0           // r
};
double plant(double in_plant){
    out_plant = (dt*(in_plant+in_plant_1)-(dt-2.4)*out_plant_1 )/(2.4+dt);
    in_plant_1 = in_plant;
    return out_plant;
}
double pid(double in, double target, double Kp, double Ki, double Kd){
    error_1 = error_0;
    error_0 = target - in;
    cumError += error_0*dt;
    rateError = (error_0-error_1)/dt;
    return Kp*error_0+Ki*cumError+Kd*rateError;
}
void IRAM_ATTR handle_set(){
  time_cnt = millis();
  if(time_cnt - last_time_cnt > 500){
    switch (state){
    case state_ready:
      value = Kp;
      state = state_confKp;
      break;
    case state_confKp:
      Kp = value; 
      value = Ki;
      state = state_confKi;
      break;
    case state_confKi:
      Ki = value;
      value = Kd;
      state = state_confKd;
      break;
    case state_confKd:
      Kd = value;
      value = 0;
      state = state_ready;
      break;
    default:
      break;
    }
    last_time_cnt = time_cnt;
  }
}
void IRAM_ATTR handle_inc(){
  if(state != state_ready){
    time_cnt = millis();
    if(time_cnt - last_time_cnt > 100){
      if(value < 10){
        value += 0.01;
      }
      last_time_cnt = time_cnt;
    }
  }
}
void IRAM_ATTR handle_dec(){
  if(state != state_ready){
    time_cnt = millis();
    if(time_cnt - last_time_cnt > 100 && value > 0){
      value -= 0.01;
      last_time_cnt = time_cnt;
    }
  }
}
void setup(){
  // put your setup code here, to run once:
	display.setBrightness(5);
	display.clear();

  preferences.begin("Kendali_PID", false);
  Kp = preferences.getFloat("Kp", 0);
  Ki = preferences.getFloat("Ki", 0);
  Kd = preferences.getFloat("Kd", 0);
  pinMode(potensio, INPUT);
  pinMode(set, INPUT_PULLUP);
  pinMode(increment, INPUT_PULLUP);
  pinMode(decrement, INPUT_PULLUP);
  attachInterrupt(set, handle_set, FALLING);
  attachInterrupt(increment, handle_inc, RISING);
  attachInterrupt(decrement, handle_dec, RISING);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  time_hold = millis();
  while((digitalRead(increment) || digitalRead(decrement))&& state != state_ready){
    if(millis() - time_hold > 500){
      if(digitalRead(increment)){
        if(value < 10){
          value += 0.5;
        }
      }
      else{
        if(value < 0.5){
          value = 0;
        }
        else{
          value -= 0.5;
        }
      }
      delay(500);
    }
    display.showNumberDecEx(value*100, 0b01000000, true, 4, 0);
  }
  if(state == state_ready){
    preferences.putFloat("Kp", Kp);
    preferences.putFloat("Ki", Ki);
    preferences.putFloat("Kd", Kd);
    display.setSegments(ready);
  }
  else{
    display.showNumberDecEx(value*100, 0b01000000, true, 4, 0);
  }
  target_kecepatan[i] = analogRead(potensio)*20/4095 - 10; // buat batas kecepatan -10 sampai 10
  i = i<10 ? i++ : 0;
  for(int j=0;j<10;j++){
    jumlah += target_kecepatan[i]; 
  }
  avr_target_kecepatan = jumlah/10;
  jumlah = 0;
  kecepatan = plant(pid(kecepatan, avr_target_kecepatan, Kp, Ki, Kd));
  out_plant_1 = kecepatan;
  Serial.print(avr_target_kecepatan);
  Serial.print(",");
  Serial.println(kecepatan);
  delay(10);
}
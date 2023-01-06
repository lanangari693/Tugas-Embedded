#include "fsm.h"
#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;
time_t time1;

bool buka = true;
double in_plant = 0, in_plant_1 = 0, in_plant_2 = 0, out_plant = 0, out_plant_1 = 0, out_plant_2 = 0;
double dt = 0.01;
double t = 0.0;
double out = 0;
double error_1 = 0, error_0 = 0;
double cumError = 0;
double rateError = 0;
int cnt=0;


void fsm(int* state, int* state_move, bool tombol_B, bool tombol_T, double* kecepatan){
    if(*state == state_tertutup){
        //*kec = 0;
        if(tombol_B){
            *state = state_sedangbuka;
        }
        else if(tombol_T){
            *state = state_tertutup;
        }
    }
    else if(*state == state_sedangbuka){

        move(state, state_move, tombol_B, kecepatan);
        time1 = time(0);
    }
    else if(*state == state_terbuka){
        if(tombol_T){
            *state = state_sedangtutup;
        }
        else if(tombol_B){
            *state = state_terbuka;
            time1 = time(0);
        }
        if(time(0)-time1 >= 10){
            *state = state_sedangtutup;
        }
    }
    else if(*state = state_sedangtutup){
        //target(&state, tombol_B, &target_v, &target_a);
        if(tombol_B){
            *state = state_sedangbuka;
            *state_move = state_accelerate;
        }
        else{
            move(state, state_move, tombol_B, kecepatan);
        }
    }

}

double plant_vel(double in_plant){
    out_plant = (dt*(in_plant+in_plant_1)-(dt-2.4)*out_plant_1 )/(2.4+dt);
    in_plant_1 = in_plant;
    return out_plant;
}
double plant_acc(double in_plant){
    out_plant = (dt*dt*(in_plant+2*in_plant_1+in_plant_2)+9.6*out_plant_1+(2*dt-4.8)*out_plant_2)/(2*dt+4.8);
    in_plant_2 = in_plant_1;
    in_plant_1 = in_plant;
    return out_plant;
}
double pid(double in, double target, double Kp, double Ki, double Kd){
    error_1 = error_0;
    error_0 = target - in;
    cumError += error_0*dt;
    rateError = (error_0-error_1)/dt;
    out = Kp*error_0+Ki*cumError+Kd*rateError;
    return out;
}
void move(int* state, int* state_move, bool tombol_B, double* kecepatan){
  if(*state == state_sedangbuka){
    if(*state_move == state_accelerate){
            *kecepatan = plant_acc(pid((*kecepatan-out_plant_2)/dt, target_acc, Kp_a, Ki_a, Kd_a));
            out_plant_2 = out_plant_1;
            out_plant_1 = *kecepatan;
            if(target_vel<*kecepatan*(1+toleransi) && target_vel >*kecepatan*(1-toleransi)){
                *state_move = state_constant;  
            }     
    }
    else if(*state_move == state_constant){
        *kecepatan = plant_vel(pid(*kecepatan, target_vel, Kp_v, Ki_v, Kd_v));
        out_plant_2 = out_plant_1;
        out_plant_1 = *kecepatan;
        if(target_vel<*kecepatan*(1+toleransi*0.1) && target_vel >*kecepatan*(1-toleransi*0.1)){
                *state_move = state_decelerate;  
        }
    }
    else if(*state_move == state_decelerate){
        *kecepatan = plant_acc(pid((out_plant_1-out_plant_2)/dt, -1*target_acc, Kp_a, Ki_a, Kd_a));
        out_plant_2 = out_plant_1;
        out_plant_1 = *kecepatan;
            if(*kecepatan<0.01){
                *state_move = state_stop;
                *kecepatan = 0;  
            }       
    }
    else if(*state_move == state_stop){
        *state=state_terbuka;
        *state_move = state_accelerate;
    }
  }
  else if(*state == state_sedangtutup){
    if(*state_move == state_accelerate){
            *kecepatan = plant_acc(pid((*kecepatan-out_plant_2)/dt, -target_acc, Kp_a, Ki_a, Kd_a));
            out_plant_2 = out_plant_1;
            out_plant_1 = *kecepatan;
            

            if(-target_vel>*kecepatan*(1+toleransi) && -target_vel <*kecepatan*(1-toleransi)){
                *state_move = state_constant;  
            }     
    }
    else if(*state_move == state_constant){
        *kecepatan = plant_vel(pid(*kecepatan, -target_vel, Kp_v, Ki_v, Kd_v));
        out_plant_2 = out_plant_1;
        out_plant_1 = *kecepatan;
        if(-target_vel>*kecepatan*(1+toleransi*0.1) && -target_vel <*kecepatan*(1-toleransi*0.1)){
                *state_move = state_decelerate;  
        }
    }
    else if(*state_move == state_decelerate){
        *kecepatan = plant_acc(pid((out_plant_1-out_plant_2)/dt, target_acc, Kp_a, Ki_a, Kd_a));
        out_plant_2 = out_plant_1;
        out_plant_1 = *kecepatan;
            if(*kecepatan>0.01){
                *state_move = state_stop;
                *kecepatan = 0;  
            }       
    }
    else if(*state_move == state_stop){
        *state=state_tertutup;
        *state_move = state_accelerate;
    }
  }
   t += dt;
}

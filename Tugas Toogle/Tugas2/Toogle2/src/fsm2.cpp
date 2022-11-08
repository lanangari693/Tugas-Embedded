#include "fsm2.h"
int count = 0;
bool out = false;
bool fsm(int* state, bool input){
    if(*state == state_0){
        if(!input){
            *state = state_0;
            count = 0;
        }
        else{
            count++;
            if(count > cnt_thrs_dbnc){
                *state = state_1;
            }
        }
        out = false;
    }
    else if(*state == state_1){
        count = 0;
        if(!input){
            *state = state_2;
        }
        else{
            *state = state_1; 
        }
    }
    else if(*state == state_2){
        if(!input){
            *state = state_2;
            count = 0;
        }
        else{
            count++;
            if(count > cnt_thrs_dbnc){
                *state = state_3;
            }
        }
        out = true;
    }
    else if(*state == state_3){
        if(!input){
            if(count < cnt_thrs_s){
                *state = state_2;
            }
            else{
                *state = state_0;
            }
            count = 0;
        }
        else{
            count++;
        }
    }
    return out;
}
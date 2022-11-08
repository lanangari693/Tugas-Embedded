#include "fsm.h"
int count = 0;
bool output = 0;
bool fsm(int* state, bool input){
    if(*state == state_0){
        if(input == 0){
            *state = state_0;
            count = 0;
        }
        else{
            count++;
            if(count > cnt_thrs){
                *state = state_1;
            }
            else{
                *state = state_0;
            }
            
        }
        output = 0;
    }
    else if(*state == state_1){
        count = 0;
        if(input == 0){
            *state = state_2;
        }
        else{
             *state = state_1;
        }
    }
    else if(*state == state_2){
        if(input == 0){
            *state = state_2;
            count = 0;
        }
        else{
            count++;
            if(count > cnt_thrs){
                *state = state_3;
                count = 0;
            }
        }
        output = 1;
    }
    else if(*state == state_3){
        count = 0;
        if(input == 0){
            *state = state_0;
        }
        else{
             *state = state_3;
        }
    }
    return output;
}
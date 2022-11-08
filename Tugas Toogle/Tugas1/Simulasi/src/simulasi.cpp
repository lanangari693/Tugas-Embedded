#include <iostream>
#include <string.h>
#include "fsm.h"
using namespace std;
bool input = false;
bool output;
int state = state_0;
string cek;
int refer[30]= {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0};
int main(){
    cout<<"No  Input  Output  Ref  Test\n";
    for(int i=0;i<30;i++){
        if(i%5==0 && i!=0){
            input = !input;
        }
        output=fsm(&state,input);
        cek = (output==refer[i])? "OK" : "fail";
        cout<<i<<"     "<<input<<"      "<<output<<"     "<<refer[i]<<"    "<<cek<<endl;
    }

    cin.get();
    return 0;
}
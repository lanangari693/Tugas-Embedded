#include <iostream>
#include <string.h>
#include "fsm2.h"
using namespace std;
bool output;
int state = state_0;
string cek;
int refer[21]= {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0};
int input[21]= {0,0,1,1,1,0,0,0,1,1,0,1,1,1,1,1,1,1,0,0,0};

int main(){
    cout<<"No  Input  Output  Ref  Test\n";
    for(int i=0;i<21;i++){
        output=fsm(&state,input[i]);
        cek = (output==refer[i])? "OK" : "fail";
        cout<<i<<"     "<<input[i]<<"      "<<output<<"     "<<refer[i]<<"    "<<cek<<endl;
    }

    cin.get();
    return 0;
}
#include <iostream>
using namespace std;
double in_plant, in_plant_1, out_plant, out_plant_1;
double dt = 0.01;
double t = 0.0;

double plant_kec(double in_plant){
    return (dt*(in_plant+in_plant_1)-(dt-2.4)*out_plant_1 )/(2.4+dt);
}
int main(){
    while(true){
        out_plant = plant_kec(1);
        in_plant_1 = 1;
        out_plant_1 = out_plant;
        t += dt;
        cout<<t<<" "<<out_plant<<endl;
        if(t>=7){
            break;
        }
    }

    cin.get();
    return 0;
}

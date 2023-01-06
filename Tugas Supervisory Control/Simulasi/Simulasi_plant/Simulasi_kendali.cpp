#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
double in_plant = 0, in_plant_1 = 0, in_plant_2 = 0, out_plant = 0, out_plant_1 = 0, out_plant_2 = 0;
double dt = 0.01;
double t = 0.0;
double out = 0;
double error_1 = 0, error_0 = 0;
double cumError = 0;
double rateError = 0;
double target = 1.0;
double Kp = 1;
double Ki = 1;
double Kd = 0;
double p=0,q=0;

double plant_kec(double in_plant){
    out_plant = (dt*(in_plant+in_plant_1)-(dt-2.4)*out_plant_1 )/(2.4+dt);
    in_plant_1 = in_plant;
    return out_plant;
}

//Plant dari transformasi bilinear
double plant_acc(double in_plant){
    out_plant = (dt*dt*(in_plant+2*in_plant_1+in_plant_2)+9.6*out_plant_1+(2*dt-4.8)*out_plant_2)/(2*dt+4.8);
    in_plant_2 = in_plant_1;
    in_plant_1 = in_plant;
    return out_plant;
}

double pid(double in){
    error_1 = error_0;
    error_0 = target - in;
    cumError += error_0*dt;
    rateError = (error_0-error_1)/dt;
    out = Kp*error_0+Ki*cumError+Kd*rateError;
    return out;
}

int main(){
    ofstream myfile;
    // untuk tes kecepatan
    //myfile.open("graph_kecepatan.txt");
    // untuk tes percepatan
    myfile.open ("graph_percepatan.txt");
    while(true){
        out_plant_2 = out_plant_1;
        out_plant_1 = out;

        //untuk tes kecepatan
        //out = plant_kec(pid(out));

        //untuk tes percepatan
        p = (out_plant_1-out_plant_2)/dt;
        q = pid(p);
        out = plant_acc(q);
        t += dt;
        cout<<t<<"  "<<out<<endl;
        

        
        myfile << t;
        myfile << ';';
        myfile << out;
        myfile << endl;
        if(t >= 10){
            break;
        }
    }
    myfile.close();

    cin.get();
    return 0;
}
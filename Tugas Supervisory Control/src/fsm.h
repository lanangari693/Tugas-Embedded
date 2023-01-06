#define state_tertutup 0 //Tertutup
#define state_sedangbuka 1 //Sedang Membuka
#define state_terbuka 2 //Terbuka
#define state_sedangtutup 3 //Sedang Menutup
#define state_accelerate 4 //Accelerate
#define state_constant 5 //Constant Speed
#define state_decelerate 6 //Decelerate
#define state_stop 7 //Stop

#define target_vel 3.0
#define target_acc 3.0
#define Kp_v 1.0
#define Ki_v 1.0
#define Kd_v 0.0
#define Kp_a 1.0
#define Ki_a 0.0
#define Kd_a 0.0

#define toleransi 0.05

#define mode_vel 0
#define mode_acc 1

void fsm(int* state, int* state_move, bool tombol_B, bool tombol_T, double* kecepatan);
double pid(double in, double target, double Kp, double Ki, double Kd);
void move(int* state, int* state_move, bool tombol_B, double* kecepatan);





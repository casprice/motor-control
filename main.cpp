#include "MotorControl.h"

const unsigned int enc_addr[] = { 0x40, 0x41, 0x42 }; // Encoder addresses
const int Kp = 0.08, Ki = 0.0, Kd = 0.0; // PID values

/**
 * main() control method
 */
int main() {
  vector<shared_ptr<Encoder>> enc_list;
  vector<shared_ptr<PID>> pid_list;
  
  for (int i = 0; i < 3; i++) {
    enc_list[i] = shared_ptr<Encoder>(new Encoder(2, enc_addr[i]));
    pid_list[i] = shared_ptr<PID>(new PID(i+1, 0, 0, 0, enc_list[i]));
  }

  MotorControl mc(enc_list, pid_list);
  mc.start();

  // ROS while loop

  mc.stop();
}
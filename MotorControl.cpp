#include <iostream>
#include <vector>
#include <memory>

#include "MotorControl.h"
#include "Encoder.hpp"
#include "PID.hpp"
#include "library/gpio.h"
#include "library/pwm.h"
#include "library/util.h"

using namespace std;

#define FREQ 20000 // 20 kHz
#define STEP 1

MotorControl::MotorControl() {
  
}

MotorControl::~MotorControl() {
  if (worker_thread.joinable()) {
    shouldStop = true;
    worker_thread.join();
  }
}

void MotorControl::start() {
  if (worker_thread.joinable()) {
    shouldStop = true;
    worker_thread.join();
  }
  shouldStop = false;
  worker_thread = thread(&MotorControl::worker, this);
}

void MotorControl::stop() {
  shouldStop = true;
}

void MotorControl::callback() {
  cerr << "timer callback run" << endl;
}

void MotorControl::worker() {
  vector<shared_ptr<Encoder>> encoders;
  vector<shared_ptr<PID>> pidctrls;
  for (int i = 0; i < 3; i++) {
    encoders[i] = shared_ptr<Encoder>(new Encoder(2, enc_addr[i]));
    pidctrls[i] = shared_ptr<PID>(new PID(i+1, Kp, Ki, Kd, encoders[i]));
  }
  while (!shouldStop) {
    this->callback();
    this_thread::sleep_for(chrono::milliseconds(100));
  }
}

int main() {}
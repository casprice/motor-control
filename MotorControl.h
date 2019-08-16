#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <thread>
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

#define NUM_MOTORS 3

// P8_13, P8_19, P9_16
int pwm_ss[] = { 2,   2,   1 };  // subsystem
int pwm_ch[] = {'B', 'A', 'B'};  // channel

// P9_15, P8_10, P8_36
int direction_vals[] = { 48, 45, 80 };

// P9_12, P8_11, P8_35
int enable_vals[] = { 60, 68, 8 };

// AIN1, AIN3, AIN5
int motor_adc[] = { 1, 3, 5 };

// Encoder addresses
unsigned int enc_addr[] = { 0x40, 0x41, 0x42 };

int Kp = 0.08,
    Ki = 0.0,
    Kd = 0.0;

class MotorControl {
  public:
    int interval;
    vector<shared_ptr<Encoder>> encoders;
    vector<shared_ptr<PID>> pidctrls;

    MotorControl();
    void start();
    void stop();
    virtual void callback();
    void worker();
    ~MotorControl();

  protected:
    bool shouldStop;
    std::thread worker_thread;
};

#endif // MOTORCONTROL_H
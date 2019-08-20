#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <thread>
#include <iostream>
#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "PID.hpp"
using namespace std;

#define NUM_MOTORS 3

class MotorControl {
  public:
    int setpoint;
    vector<shared_ptr<Encoder>> encoder_list;
    vector<shared_ptr<PID>> pidctrl_list;

    MotorControl(vector<shared_ptr<Encoder>> enc_list, 
                 vector<shared_ptr<PID>> pid_list);
    ~MotorControl();
    void start();
    void stop();
    virtual void callback();
    void worker();

  protected:
    bool shouldStop;
    std::thread worker_thread;
};

#include "MotorControl.cpp"

#endif // MOTORCONTROL_H
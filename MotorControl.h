#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <thread>
#include <iostream>
#include <vector>
#include <memory>

#include "Encoder.hpp"
#include "PID.hpp"
using namespace std;

class MotorControl {
  public:
    vector<shared_ptr<Encoder>> encoder_list;
    vector<shared_ptr<PID>> pidctrl_list;

    MotorControl(float a_dt);
    ~MotorControl();

    void start();
    void stop();
    virtual void callback();
    void worker();

  protected:
    bool shouldStop;
    std::thread worker_thread;

  private:
    int setpoint;
    chrono::microseconds dt;
};

#include "MotorControl.cpp"

#endif // MOTORCONTROL_H
/**
 * File: PID.hpp
 */

#ifndef PID_HPP
#define PID_HPP

#include <memory>   // shared_ptr
#include <rc/adc.h>
#include <rc/math/filter.h>
using namespace std;

#include "Encoder.hpp"
#include "library/pwm.h"
#include "library/gpio.h"

// P8_15, P9_12, P8_36
const int direction_vals[] = { 47, 60, 80 };
// P8_14, P9_11, P8_35
const int enable_vals[] = { 26, 30, 8 };
// AIN1, AIN3, AIN5
const int motor_adc[] = { 1, 3, 5 };

class PID {
  private:
    double dutyCycle;
    double goalAngle;
    shared_ptr<Encoder> encoder;
    rc_filter_t filter;
    PWM* pwmPin;
    GPIO* dirPin;
    GPIO* enablePin;

  public:
    PID(int motorNum, 
        float dt,
        double Kp=0, 
        double Ki=0, 
        double Kd=0, 
        shared_ptr<Encoder> enc=NULL);
    PID(int motorNum);
    
    void updatePWM(bool invert);
    void setAngle(double angle);
    double getDutyCycle(void);
    int setDuty(double duty);
    double getCurrent(int ch);
    double getTorque(int ch);
    void setTorque(void);
    void setAngle(void);
    double clip(double number, int min, int max, int err);

    ~PID(void);
};

#include "PID.cpp"

#endif

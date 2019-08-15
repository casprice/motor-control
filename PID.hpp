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
#include "library/gpio.h"
#include "library/pwm.h"


class PID {
    private:
        double dutyCycle;
        shared_ptr<Encoder> encoder;
        rc_filter_t filter;

    public:
        PID(double Kp=0, double Ki=0, double Kd=0, shared_ptr<Encoder> enc=NULL);
        
        void updatePWM(PWM* pwm, GPIO* pin, int goalAngle, bool invert);
        double getDutyCycle(void);
        double getCurrent(int ch);
        double getTorque(int ch);
        void setTorque(void);
        void setAngle(void);
        double clip(double number, int min, int max, int err);

        ~PID(void);
};

#include "PID.cpp"

#endif
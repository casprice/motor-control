/**
 * File: PID.hpp
 */

#ifndef PID_HPP
#define PID_HPP

#include "library/gpio.h"
#include "library/pwm.h"

class PID {
    private:
        double Kp;
        double Ki;
        double Kd;
        double totalError;
        double prevError;
        double dutyCycle;

    public:
        PID(double Kp=0, double Ki=0, double Kd=0);
        ~PID(void);
        
        void updatePWM(PWM& pwm, double current, int goal);
        void updatePin(GPIO& pin, bool invert);
        void clearKi();
};

#include "PID.cpp"

#endif
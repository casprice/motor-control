/**
 * File: PID.hpp
 */

#ifndef PID_HPP
#define PID_HPP

#include "library/gpio.h"
#include "library/pwm.h"

using namespace exploringBB;

class PID {
    private:
        double Kp;
        double Ki;
        double Kd;
        double totalError;
        double prevError;

    public:
        PID(double Kp, double Ki, double Kd);
        ~PID(void);
        
        void update(GPIO& dir, PWM& pwm, double current, int goal, bool invertDir);
        void clearKi();
};

#include "PID.cpp"

#endif
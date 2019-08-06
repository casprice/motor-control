/**
 * File: PID.hpp
 */

#ifndef PID_HPP
#define PID_HPP

#include <memory>
using namespace std;

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
        
        void updatePWM(PWM& pwm, shared_ptr<Encoder> encoder, int goal);
        void updatePin(GPIO& pin, bool invert);
        void clearKi(void);

        ~PID(void);
};

#include "PID.cpp"

#endif
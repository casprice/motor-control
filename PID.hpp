/**
 * File: PID.hpp
 */

#ifndef PID_HPP
#define PID_HPP

#include <memory>
using namespace std;

#include "Encoder.hpp"
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
        shared_ptr<Encoder> encoder;

    public:
        PID(double Kp=0, double Ki=0, double Kd=0, shared_ptr<Encoder> enc);
        
        void updatePWM(PWM& pwm, int goal);
        void updatePin(GPIO& pin, bool invert);
        void clearKi(void);

        ~PID(void);
};

#include "PID.cpp"

#endif
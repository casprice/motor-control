/**
 * File: PID.cpp
 */

//#include <rc/gpio.h>
#include <cmath> // abs()

#include "PID.hpp"
#include "Driver.hpp"

#define MIN_DUTY 0
#define MAX_DUTY 100

// Default ctor
PID :: PID(double a_Kp, double a_Ki, double a_Kd) {
    Kp = a_Kp;
    Ki = a_Ki;
    Kd = a_Kd;
    totalError = 0;
    prevError = 0;
}

// Default dtor
PID :: ~PID(void) {
    return;
}

/**
 * 
 */
void PID :: update(GPIO& dir, PWM& pwm, double current, int goal, bool invertDir) { 
    float adjustedDuty = (Kp * (goal - current)) + Ki * totalError + Kd * prevError;
    adjustedDuty = clip(abs(adjustedDuty), MIN_DUTY, MAX_DUTY);
    
    // Direction
    if((adjustedDuty < 0) != invertDir) {
        dir.setValue(GPIO::HIGH);
    } else {
        dir.setValue(GPIO::LOW);
    }

    // Enable
    pwm.setDutyCycle(adjustedDuty);

    totalError += goal - current;
    prevError += goal - current;
}

/**
 * 
 */
void PID :: clearKi() {
    totalError = 0;
}
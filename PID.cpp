/**
 * File: PID.cpp
 */

#include <rc/gpio.h>
#include <cmath> // abs()

#include "PID.hpp"
#include "Driver.hpp"

#define MIN_DUTY 0
#define MAX_DUTY 100

// Default ctor
PID :: PID(void) {
    Kp = 0;
    Ki = 0;
    Kd = 0;
    totalError = 0;
    prevError = 0;
}

// Copy ctor
PID :: PID(double a_Kp, double a_Ki, double a_Kd) {
    Kp = a_Kp;
    Ki = a_Ki;
    Kd = a_Kd;
    totalError = 0;
    prevError = 0;
}

// Default dtor
PID :: ~PID(void) {}

/**
 * 
 */
void PID :: update(double current, int goal, bool invertDir=false) {
    rc_gpio_init(DIR_CHIP, DIR_PIN, GPIOHANDLE_REQUEST_OUTPUT);
    double adjustedDuty = (Kp * (goal - current)) + Ki * totalError + Kd * prevError;
    adjustedDuty = clip(abs(adjustedDuty), MIN_DUTY, MAX_DUTY);
    
    // Direction
    if((adjustedDuty < 0) != invertDir) {
        rc_gpio_set_value(DIR_CHIP, DIR_PIN, HIGH);
    } else {
        rc_gpio_set_value(DIR_CHIP, DIR_PIN, LOW);
    }

    // Enable
    rc_pwm_set_duty(PWM_SUBSYS, PWM_CH_A, adjustedDuty/100);
    rc_pwm_set_duty(PWM_SUBSYS, PWM_CH_B, adjustedDuty/100);

    totalError += goal - current;
    prevError += goal - current;
}

/**
 * 
 */
void PID :: clearKi() {
    totalError = 0;
}
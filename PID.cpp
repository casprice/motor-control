/**
 * File: PID.cpp
 */

#include <rc/gpio.h>
#include <cmath> // abs()

#include "PID.hpp"
#include "Driver.hpp"

#define MIN_DUTY 0
#define MAX_DUTY 100

double Kp = 0;
double Ki = 0;
double Kd = 0;
double totalError = 0;
double prevError = 0;

// Default ctor
PID::PID() {}

// Copy ctor
PID::PID(double Kp, double Ki, double Kd) {
    this.Kp = Kp;
    this.Ki = Ki;
    this.Kd = Kd;
}

// Default dtor
PID::~PID() {}

/**
 * 
 */
void PID::update(double current, int goal, bool invertDir=false) {
    double adjustedDuty = (Kp * (goal - current)) + Ki * totalError + Kd * prevError;
    adjustedDuty = clip(abs(adjustedDuty), MIN_DUTY, MAX_DUTY);
    
    // Direction
    if((adjustedDuty < 0) != invertedDir) {
        rc_gpio_set_value(DIR_CHIP, DIR_PIN, HIGH);
    } else {
        rc_gpio_set_value(DIR_CHIP, DIR_PIN, LOW);
    }

    // Enable
    rc_pwm_set_duty(PWM_SUBSYS, PWM_CH_A, adjustedDuty);
    rc_pwm_set_duty(PWM_SUBSYS, PWM_CH_B, adjustedDuty);

    totalError += goal - current;
    prevError += goal - current;
}

/**
 * 
 */
void PID::clearKi() {
    totalError = 0;
}
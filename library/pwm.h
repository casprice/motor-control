#ifndef PWM_H_
#define PWM_H_

#define NUM_MOTORS 3

// P8_13, P8_19, P9_16
const int pwm_ss[] = { 2,   2,   1 };  // subsystem
const int pwm_ch[] = {'B', 'A', 'B'};  // channel

/**
 * @class PWM
 * @brief A class to control a basic PWM output
 */
class PWM {
  private:
    int num;  // which motor we're controlling

  public:
    PWM(int motorNum, int freq);
    int setDutyCycle(double duty);
    ~PWM();
};

#include "pwm.cpp"

#endif /* PWM_H_ */
#ifndef PWM_H_
#define PWM_H_

#define NUM_MOTORS 3

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
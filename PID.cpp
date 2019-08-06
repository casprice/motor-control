/**
 * File: PID.cpp
 * Description: TODO
 */

#include <cmath> // abs()

#include "PID.hpp"
#include "Driver.hpp"

#define MIN_DUTY 0
#define MAX_DUTY 100

/**
 * Routine name: PID(double a_Kp, double a_Ki, double a_Kd)
 * Description: Copy constructor for PID. Assigns the PID values and initializes 
 *              error values to 0.
 * Parameters: a_Kp - PID proportion argument
 *             a_Ki - PID integral argument
 *             a_Kd - PID derivative argument
 */
PID(double a_Kp, double a_Ki, double a_Kd, shared_ptr<Encoder> enc) {
  Kp = a_Kp;
  Ki = a_Ki;
  Kd = a_Kd;
  encoder = enc;
  totalError = 0;
  prevError = 0;
  dutyCycle = 0;
}

/**
 * Routine name: updatePWM(PWM& pwm, double current, int goal)
 * Description: Configures the PWM pin for output by adjusting the duty cycle
 *              through PID control based on current angle and goal.
 * Parameters: pwm - reference to PWM pin
 *             current - current angle as calculated by encoder
 *             goal - angle we want to move the motor to
 * Return value: None.
 */
void updatePWM(PWM& pwm, int goal) {
  // check that goal should not exceed limits

  double current = encoder.getAngle();

  // Calculate the new duty cycle value from PID control
  dutyCycle = (Kp * (goal - current)) + Ki * totalError + Kd * prevError;

  // Ensure new duty cycle doesn't exceed min or max possible values
  dutyCycle = clip(abs(dutyCycle), MIN_DUTY, MAX_DUTY);
  
  // Set new duty cycle
  pwm.setDutyCycle(dutyCycle);

  // Adjust error
  totalError += goal - current;
  prevError += goal - current;
}

/**
 * Routine name: updatePin(PWM& pwm, double current, int goal)
 * Description: Configures a GPIO pin for output. Depending on value of invert,
 *              set value to high or low.
 * Parameters: pin - reference to GPIO pin
 *             invert - whether to invert pin
 * Return value: None.
 */
void updatePin(GPIO& pin, bool invert) {
  if((dutyCycle < 0) != invert) {
    pin.setValue(GPIO::HIGH);
  } else {
    pin.setValue(GPIO::LOW);
  }
}

/**
 * Routine name: clearKi(void)
 * Description: Zeroes-out integral part of PID.
 * Parameters: None.
 * Return value: None.
 */
void clearKi(void) {
  totalError = 0;
}

/**
 * Routine name: ~PID(void)
 * Description: Default desctructor for PID.
 * Parameters: None.
 */
~PID(void) {}
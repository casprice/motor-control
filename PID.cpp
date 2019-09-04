/**
 * File: PID.cpp
 * Description: TODO
 */

#include <cmath> // fabs()

#include "PID.hpp"

#define FREQ 20000 // 20 kHz
#define MIN_DUTY 0.0
#define MAX_DUTY 0.9
#define MIN_ANGLE -95
#define MAX_ANGLE 95
#define TORQUE_CONST 6.18 // mNm/A
#define GEAR_RATIO 44     // 44:1
#define GEARBOX_EFF 81    // 81% efficiency

/**
 * Routine name: PID(double a_Kp, double a_Ki, double a_Kd)
 * Description: Copy constructor for PID. Assigns the PID values and initializes
 *              error values to 0.
 * Parameters: a_Kp - PID proportion argument
 *             a_Ki - PID integral argument
 *             a_Kd - PID derivative argument
 */
PID::PID(int motorNum, double Kp, double Ki, double Kd, shared_ptr<Encoder> enc) {
  // Initialize pins
  pwmPin = new PWM(motorNum, FREQ);
  dirPin = new GPIO(direction_vals[motorNum-1]);
  enablePin = new GPIO(enable_vals[motorNum-1]);
  dutyCycle = 0;
  encoder = enc;
  filter = RC_FILTER_INITIALIZER;
  // Initialize PID filter
  if (rc_filter_pid(&filter, Kp, Ki, Kd, 4*DT, DT)) {
    cerr << "ERROR: failed to run rc_filter_pid()\n";
  }
  // Initialize analog-to-digital converter
  if (rc_adc_init()) {
    cerr << "ERROR: failed to run rc_init_adc()\n";
  }
}

/**
 * Routine name: updatePWM(PWM& pwm, double current, int goal)
 * Description: Configures the PWM pin for output by adjusting the duty cycle
 *              through PID control based on current angle and goal.
 * Parameters: pwm - reference to PWM pin
 *             dir - the GPIO direction pin
 *             goalAngle - angle we want to move the motor to
 *             invert - whether to invert the direction of the motor
 * Return value: None.
 */
void PID::updatePWM(int goalAngle, bool invert) {
  // Check that goal angle does not exceed limits
  clip(goalAngle, MIN_ANGLE, MAX_ANGLE, 1);

  // Calculate the new duty cycle value from PID control
  dutyCycle = rc_filter_march(&filter, goalAngle - encoder->getAngle())/100;

  // Adjusts direction depending on the sign of dutyCycle
  if((dutyCycle < 0) != invert) {
    dirPin->setValue(GPIO::HIGH);
  } else {
    dirPin->setValue(GPIO::LOW);
  }

  // Set new duty cycle using the magnitude
  setDuty(fabs(dutyCycle));
}

/**
 * Routine name: getDutyCycle(void)
 * Description:
 * Parameters: None
 * Return value:
 */
double PID::getDutyCycle(void) {
  return dutyCycle;
}

/**
 *
 */
int PID:: setDuty(double duty) {
  dutyCycle = duty;
  return pwmPin->setDutyCycle(duty);
}

/**
 * Routine name: getCurrent(int ch)
 * Description:
 * Parameters:
 * Return value:
 */
double PID::getCurrent(int ch) {
  return rc_adc_read_volt(ch) * 1681/681 * 3.4/4;
}

/**
 * 3 - AIN0
 * 4 - AIN1
 * 5 - AIN2
 * 6 - AIN3
 */
double PID::getTorque(int ch) {
  double current = rc_adc_read_volt(ch) * 1681/681 * 3.4/4;

  // Multiply by torque constant * gear ratio * gearbox efficiency
  return current * (TORQUE_CONST/1000) * GEAR_RATIO * (GEARBOX_EFF/100);
  //return rc_adc_read_volt(ch);
}

/**
 * Routine name: clip(double number, int min, int max, int err)
 * Description: Ensure number doesn't exceed min or max values.
 * Parameters: number - the number to clip
 *             min - the minimum possible value
 *             max - the maximum possible value
 *             err - whether to print an error message
 * Return value: number as a clipped value
 */
double PID::clip(double number, int min, int max, int err) {
  if (number < min) {
    if (err) cerr << "\nInvalid value: " << number << " cannot be lower than " << min;
    number = min;
  }
  if (number > max) {
    if (err) cerr << "\nInvalid value: " << number << " cannot be higher than " << max;
    number = max;
  }

  return number;
}

/**
 * Routine name: ~PID(void)
 * Description: Default desctructor for PID.
 * Parameters: None.
 */
PID::~PID(void) {
  rc_filter_free(&filter);
  rc_adc_cleanup();
  delete pwmPin;
  delete dirPin;
  delete enablePin;
}

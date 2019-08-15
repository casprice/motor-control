/**
 * File: PID.cpp
 * Description: TODO
 */

#include <cmath> // fabs()

#include "PID.hpp"

#define MIN_DUTY 0.0
#define MAX_DUTY 1.0
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
PID::PID(double Kp, double Ki, double Kd, shared_ptr<Encoder> enc) {
  encoder = enc;
  dutyCycle = 0;
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
double PID::updatePWM(PWM* pwm, GPIO* dir, int goalAngle, bool invert) {
  // check that goal should not exceed limits

  // Calculate the new duty cycle value from PID control
  dutyCycle = rc_filter_march(&filter, goalAngle - encoder->getAngle())/100;

  // Adjusts direction depending on the sign of dutyCycle
  if((dutyCycle < 0) != invert) {
    dir->setValue(GPIO::HIGH);
  } else {
    dir->setValue(GPIO::LOW);
  }
  
  // Set new duty cycle using the magnitude
  pwm->setDutyCycle(clip(fabs(dutyCycle), MIN_DUTY, MAX_DUTY));
  return dutyCycle;
}

/**
 * 
 */
double PID::getDutyCycle(void) {
  return dutyCycle;
}

/**
 * 
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
  int i;
  for(i=0;i<7;i++){
    printf("%6.2f |", rc_adc_read_volt(i));
  }
  // Multiply by torque constant * gear ratio * gearbox efficiency
  return current * (TORQUE_CONST/1000) * GEAR_RATIO * (GEARBOX_EFF/100);
  //return rc_adc_read_volt(ch);
}

double PID::getCurrent(int ch) {
  return rc_adc_read_volt(ch) * 1681/681 * 3.4/4;
}

/**
 * Ensure number doesn't exceed min or max angles
 */
double PID::clip(double number, int min, int max) {
  if (number < min) number = min;
  if (number > max) number = max;
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
}

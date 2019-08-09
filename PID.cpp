/**
 * File: PID.cpp
 * Description: TODO
 */

#include <cmath> // abs()

#include "PID.hpp"

#define MIN_DUTY 0
#define MAX_DUTY 100
#define TORQUE_CONST 6.18 // mNm/A
#define GEAR_RATIO 33     // 33:1
#define GEARBOX_EFF 81    // 81% efficiency

/**
 * Routine name: PID(double a_Kp, double a_Ki, double a_Kd)
 * Description: Copy constructor for PID. Assigns the PID values and initializes 
 *              error values to 0.
 * Parameters: a_Kp - PID proportion argument
 *             a_Ki - PID integral argument
 *             a_Kd - PID derivative argument
 */
PID::PID(double a_Kp, double a_Ki, double a_Kd, shared_ptr<Encoder> enc) {
  Kp = a_Kp;
  Ki = a_Ki;
  Kd = a_Kd;
  encoder = enc;
  totalError = 0;
  prevError = 0;
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
 *             current - current angle as calculated by encoder
 *             goal - angle we want to move the motor to
 * Return value: None.
 */
void PID::updatePWM(PWM* pwm, int goalAngle) {
  // check that goal should not exceed limits

  //double current = (encoder.get()->getAngle());
  double currentAngle = 20;
  cout << "rc_filter_march: " << rc_filter_march(&filter, goalAngle - currentAngle) << " ";

  // Calculate the new duty cycle value from PID control
  dutyCycle = (Kp * (goalAngle - currentAngle)) + Ki * totalError + Kd * prevError;

  // Ensure new duty cycle doesn't exceed min or max possible values
  dutyCycle = PID::clip(abs(dutyCycle), MIN_DUTY, MAX_DUTY);
  
  // Set new duty cycle
  pwm->setDutyCycle(dutyCycle);

  // Adjust error
  //totalError += goal - current;
  //prevError += goal - current;
}

/**
 * Routine name: updatePin(GPIO* pin, bool invert)
 * Description: Configures a GPIO pin for output. Depending on value of invert,
 *              set value to high or low.
 * Parameters: pin - reference to GPIO pin
 *             invert - whether to invert pin
 * Return value: None.
 */
void PID::updatePin(GPIO* pin, bool invert) {
  if((dutyCycle < 0) != invert) {
    pin->setValue(GPIO::HIGH);
  } else {
    pin->setValue(GPIO::LOW);
  }
}

double PID::getCurrent(int ch) {
  return rc_adc_read_volt(ch) * 1681/681 * 3.4/4;
}

/**
 * 
 * 
 */
double PID::getTorque(int ch) {
  double current = rc_adc_read_volt(ch) * 1681/681 * 3.4/4;
  return current * (TORQUE_CONST/1000) * (GEARBOX_EFF/100);
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
}
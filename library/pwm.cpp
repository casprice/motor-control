
#include <iostream>
#include <rc/pwm.h>

#include "pwm.h"

/**
 * Routine name: PWM(int motorNum, int freq)
 * Description: Sets the motor number and initializes the subsystem.
 * Parameters: motorNum - motor number to correspond to correct PWM pin
 *             freq - the PWM frequency
 */
PWM::PWM(int motorNum, int freq) {
	num = motorNum;
	rc_pwm_init(ss[num-1], freq); 
}

/**
 * Routine name: setDutyCycle(double duty)
 * Description: Sets the duty cycle for the PWM pin.
 * Parameters: duty - the duty cycle to send, a double between 0 and 1
 * Return value: -1 if an error occurred, 0 otherwise
 */
int PWM::setDutyCycle(double duty) {
	if (duty > 1.0) duty = 1.0;
	else if (duty < 0.0) duty = 0.0;

	if (rc_pwm_set_duty(ss[num-1], ch[num-1], duty) == -1) {
		cerr << "Error in setDutyCycle for motor #" << num << endl;
		return -1;
	}

	return 0;
}

/**
 * Routine name: ~PWM(void)
 * Description: Destructor for the PWM. Cleans up the subsystem.
 * Parameters: None
 */
PWM::~PWM(void) {
	rc_pwm_cleanup(ss[num-1]);
}
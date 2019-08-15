
#include <iostream>
#include <rc/pwm.h>

#include "pwm.h"
#include "util.h"

#define MAX_BUF 64

PWM::PWM(int freq) {
	rc_pwm_init(1, freq); // pwm subsystem 1
	//rc_pwm_init(2, freq); // pwm subsystem 2
}

int PWM::setDutyCycle(double duty) {
	if (duty > 1.0) duty = 1.0;
	else if (duty < -1.0) duty = -1.0;
	// P8_13
	/*if (rc_pwm_set_duty(2, 'B', duty) == -1) {
		cerr << "Error in setDutyCycle 1" << endl;
		return -1;
	}
	// P8_19
	if (rc_pwm_set_duty(2, 'A', duty) == -1) {
		cerr << "Error in setDutyCycle 2" << endl;
		return -1;
	}*/
	// P9_16
	if (rc_pwm_set_duty(1, 'B', duty) == -1) {
		cerr << "Error in setDutyCycle 3" << endl;
		return -1;
	}
	return 0;
}

PWM::~PWM() {
	rc_pwm_cleanup(1);
	//rc_pwm_cleanup(2);
}
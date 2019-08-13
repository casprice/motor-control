
#include <iostream>
#include <rc/pwm.h>

#include "pwm.h"
#include "util.h"

#define MAX_BUF 64
static int pwmss[4];
static int pwmch[4];

PWM::PWM(int freq) {
	pwmss[0] = 1;
	pwmch[0] = 'A';
	pwmss[1] = 1;
	pwmch[1] = 'B';
	pwmss[2] = 2;
	pwmch[2] = 'A';
	pwmss[3] = 2;
	pwmss[3] = 'B';
	rc_pwm_init(1, freq); // pwm subsystem 1
	rc_pwm_init(2, freq); // pwm subsystem 2
}

int PWM::setDutyCycle(double duty) {
	if (duty > 1.0) duty = 1.0;
	else if (duty < -1.0) duty = -1.0;
	/*if (rc_pwm_set_duty(pwmss[0], pwmch[0], duty) == -1) {
		cerr << "Error in setDutyCycle on ss" << pwmss[0] << " and ch" << pwmch[0] << endl;
		return -1;
	}
	if (rc_pwm_set_duty(pwmss[2], pwmch[2], duty) == -1) {
		cerr << "Error in setDutyCycle on ss" << pwmss[2] << " and ch" << pwmch[2] << endl;
		return -1;
	}*/
	if (rc_pwm_set_duty(1, 'B', duty) == -1) {
		cerr << "Error in setDutyCycle on ss1 and chB" << endl;
		return -1;
	}
	return 0;
}

PWM::~PWM() {
	rc_pwm_cleanup(1);
	rc_pwm_cleanup(2);
}
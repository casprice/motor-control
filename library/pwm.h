/*
 * PWM.h  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone" 
 * See: www.exploringbeaglebone.com
 * Licensed under the EUPL V.1.1
 *
 * This Software is provided to You under the terms of the European 
 * Union Public License (the "EUPL") version 1.1 as published by the 
 * European Union. Any use of this Software, other than as authorized 
 * under this License is strictly prohibited (to the extent such use 
 * is covered by a right of the copyright holder of this Software).
 * 
 * This Software is provided under the License on an "AS IS" basis and 
 * without warranties of any kind concerning the Software, including 
 * without limitation merchantability, fitness for a particular purpose, 
 * absence of defects or errors, accuracy, and non-infringement of 
 * intellectual property rights other than copyright. This disclaimer 
 * of warranty is an essential part of the License and a condition for 
 * the grant of any rights to this Software.
 * 
 * For more details, see http://www.derekmolloy.ie/
 */

#ifndef PWM_H_
#define PWM_H_
#include <string>
using std::string;

//#define PWM_PATHH "/sys/devices/platform/ocp/48300000.epwmss/48300200.pwm/pwm/pwmchip1/pwm-1:0/"
#define PWM_PATH "/sys/class/pwm/pwm-1:0/"
#define PWM_PERIOD "period"
#define PWM_DUTY "duty_cycle"
#define PWM_POLARITY "polarity"
#define PWM_RUN "run"

/**
 * @class PWM
 * @brief A class to control a basic PWM output -- you must know the exact sysfs filename
 * for the PWM output.
 */
class PWM {
public:
	enum POLARITY{ ACTIVE_HIGH=0, ACTIVE_LOW=1 };

private:
	string name, path;
	float analogFrequency;  //defaults to 100,000 Hz
	float analogMax;        //defaults to 3.3V

public:
	PWM(string pinName);
	
	int setPeriod(unsigned int period_ns);
	unsigned int getPeriod();
	int setFrequency(float frequency_hz);
	float getFrequency();
	int setDutyCycle(unsigned int duration_ns);
	int setDutyCycle(double percentage);
	unsigned int getDutyCycle();
	float getDutyCyclePercent();

	int setPolarity(PWM::POLARITY);

	void setAnalogFrequency(float frequency_hz) { this->analogFrequency = frequency_hz; }
	int calibrateAnalogMax(float analogMax); //must be between 3.2 and 3.4
	int analogWrite(float voltage);

	int run();
	bool isRunning();
	int stop();

	void export_pwms(void);

	~PWM();

private:
	float period_nsToFrequency(unsigned int);
	unsigned int frequencyToPeriod_ns(float);
};

#include "pwm.cpp"

#endif /* PWM_H_ */
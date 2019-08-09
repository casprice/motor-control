/*
 * @file GPIO.h
 * @author Derek Molloy
 * @version 0.1
 *
 * Created on: 29 Apr 2014
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

#ifndef GPIO_H_
#define GPIO_H_
#include<string>
#include<fstream>
using std::string;
using std::ofstream;

#define GPIO_PATH "/sys/class/gpio/"

typedef int (*CallbackType)(int);

/**
 * @class GPIO
 * @brief GPIO class for input and output functionality on a single GPIO pin
 */
class GPIO {
public:
	enum DIRECTION{ INPUT, OUTPUT };
	enum VALUE{ LOW=0, HIGH=1 };

private:
	int number;			/**< The GPIO number of the object */
	int debounceTime;   /**< The debounce time in milliseconds */
	string name;		/**< The name of the GPIO e.g. gpio50 */
	string path;  		/**< The full path to the GPIO e.g. /sys/class/gpio/gpio50/ */

public:
	GPIO(int number);
	int getNumber() { return number; } /**< Returns the GPIO number as an int. */

	// General Input and Output Settings
	int setUpPin(void);
	int setDirection(GPIO::DIRECTION);
	int setValue(GPIO::VALUE);
	GPIO::VALUE getValue();
	GPIO::DIRECTION getDirection();
	int exportGPIO();
	int unexportGPIO();

	~GPIO();  //destructor will unexport the pin

private:
	//int write(string path, string filename, string value);
	//int write(string path, string filename, int value);
	//string read(string path, string filename);
	
	ofstream stream;
	pthread_t thread;
	CallbackType callbackFunction;
	bool threadRunning;
	int togglePeriod;  //default 100ms
	int toggleNumber;  //default -1 (infinite)
};

#include "gpio.cpp"

#endif /* GPIO_H_ */
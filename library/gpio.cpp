/*
 * GPIO.cpp  Created on: 29 Apr 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring BeagleBone"
 * If you use this code in your work please cite:
 *   Derek Molloy, "Exploring BeagleBone: Tools and Techniques for Building
 *   with Embedded Linux", Wiley, 2014, ISBN:9781118935125.
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

#include "gpio.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <pthread.h>
using namespace std;

/**
 * Routine name: GPIO(int number)
 * Description: The constructor will set up the states and export the pin.
 * Parameters: number - The GPIO number to be exported
 */
GPIO::GPIO(int number) {
  this->number = number;

  ostringstream s;
  s << "gpio" << number;
  this->name = string(s.str());
  this->path = GPIO_PATH + this->name + "/";

  //this->unexportGPIO();
  // Export GPIO pin number
  /*if (exportGPIO() == -1) {
    cerr << "Unable to export GPIO pin " << this->number;
  }*/
  // need to give Linux time to set up the sysfs structure
  usleep(250000); // 250ms delay

  // Prepare the pin for use
  if (setUpPin() == -1) {
    cerr << "Unable to set up GPIO pin " << this->number;
  }
}

/**
 * Routine name:
 * Description:
 * Parameters:
 * Return value:
 */
int GPIO::setUpPin(void) {
  if (setDirection(OUTPUT) == -1) {
    return -1;
  }
  if (setValue(HIGH) == -1) {
    return -1;
  }
  return 0;
}

/**
 * Routine name: exportGPIO(void)
 * Description: Private method to export the GPIO
 * Parameters: None
 * Return value: int that describes if the operation fails
 */
int GPIO::exportGPIO(void) {
  return write(GPIO_PATH, "export", this->number);
}

/**
 * Routine name:
 * Description:
 * Parameters:
 * Return value:
 */
int GPIO::unexportGPIO(void) {
  return write(GPIO_PATH, "unexport", this->number);
}

/**
 * Routine name:
 * Description:
 * Parameters:
 * Return value:
 */
int GPIO::setDirection(DIRECTION dir){
  switch(dir){
  case INPUT: 
    return write(this->path, "direction", "in");
  case OUTPUT: 
    return write(this->path, "direction", "out");
  }
  return -1;
}

/**
 * Routine name:
 * Description:
 * Parameters:
 * Return value:
 */
int GPIO::setValue(VALUE value){
  switch(value){
  case HIGH: 
    return write(this->path, "value", "1");
  case LOW: 
    return write(this->path, "value", "0");
  }
  return -1;
}

/**
 * Routine name:
 * Description:
 * Parameters:
 * Return value:
 */
GPIO::VALUE GPIO::getValue(){
  string input = read(this->path, "value");
  if (input == "0") {
    return LOW;
  } else {
    return HIGH;
  }
}

/**
 * Routine name:
 * Description:
 * Parameters:
 * Return value:
 */
GPIO::DIRECTION GPIO::getDirection(){
  string input = read(this->path, "direction");
  if (input == "in") {
    return INPUT;
  } else {
    return OUTPUT;
  }
}

/**
 * Routine name:
 * Description:
 * Parameters:
 */
GPIO::~GPIO() {
  if (this->getValue() == GPIO::HIGH) {
    this->setValue(LOW);
  }
  if (this->getDirection() == GPIO::OUTPUT) {
    this->setDirection(INPUT);
  }
}
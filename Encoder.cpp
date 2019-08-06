/**
 * File: Encoder.cpp
 * Description: TODO
 */
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
using namespace std;

#include "Encoder.hpp"

#define RAW_TO_DEG 1
#define RAW_TO_RAD 2
#define DEG_TO_RAW 3
#define DEG_TO_RAD 4
#define RAD_TO_DEG 5
#define MIN_VALUE -180
#define MAX_VALUE 180

/**
 * Routine name: Encoder::Encoder(unsigned int bus, unsigned int address) (public)
 * Description: Default constructor for Encoder. Initializes the Encoder values to 
 *              zero. 
 * Parameters: a_bus - the bus number of the device
 *             a_address - the address ID
 *             a_resolution - the 14-bit resolution
 */
Encoder::Encoder(unsigned int a_bus, unsigned int a_address, double a_resolution) 
        : I2CDevice(a_bus, a_address) {
  bus = a_bus;
  address = a_address;
  resolution = a_resolution;
  angle = 0;
  zeroPosition = 0;
  setZeroPosition();
}

/**
 * Routine name: Encoder::setZeroPosition(void) (public)
 * Description: Set the zero position of the angle.
 * Parameters: None.
 * Return value: -1 if error occurred, 0 if successful.
 */
void Encoder::setZeroPosition(void) {
  calcRotation();
  zeroPosition = angle;
}

/**
 * Routine name: Encoder::calcRotation(void)
 * Description: Get angle by reading angle registers, which outputs values
 *              including zero position correction. Convert angle to degrees
 * 							according to its resolution.
 * Parameters: None.
 * Return value: NOne.
 */
void Encoder::calcRotation(void) {
  unsigned char * result = readRegisters(2, ANGLMSB_REG);
  // Convert angle to degrees within the bounds of -180 to 180 degrees
  angle = convertNum(toDecimal(result), RAW_TO_DEG) + 180 - zeroPosition;

  /* Ensure angle is within boundaries
  if (angle < MIN_VALUE) {
    cerr << "Exceeded min angle! Angle at: " << angle;
    angle = MIN_VALUE;
  }

  if (angle > MAX_VALUE) {
    angle = MAX_VALUE;
    cerr << "Exceeded max angle! Angle at: " << angle;
  }*/
}

/**
 * Routine name: Encoder::getAngle(void)
 * Description: Returns the current angle position in degrees.
 * Parameters: None.
 * Return value: The angle position.
 */
double Encoder::getAngle(void) {
  return angle;
}

/**
 * Routine name: Encoder::getZero(void)
 * Description: Returns the current zero position in degrees.
 * Parameters: None.
 * Return value: The zero position.
 */
double Encoder::getZero(void) {
  return zeroPosition;
}

/**
 * Routine names: Encoder::toDecimal(unsigned char * buf)
 * Description: Combines two 8-bit registers into a 16-bit short according to 
 *              the bits used in each register. 
 * Parameters: buf - an unsigned char * buffer containing the individual bytes.
 * Return value: 16-bit short from the two 8-bit registers.
 */
short Encoder::toDecimal(unsigned char* buf) {
  // buf[0] uses 6 bits and buf[1] uses 8 bits.
  return ((short)buf[1] << 6) | ((short)buf[0] & 0x3F);
}

/**
 * Routine name: Encoder::convertNum(double num, int conversion)
 * Description: Convert num to readable unit using device's resolution.
 * Parameters: num - the number to convert
 *             conversion - integer indicating the type of conversion to peform
 * Return value: The converted number. Defaults to returning parametrized num.
 */
double Encoder::convertNum(double num, int conversion) {
  switch(conversion) {
    // Convert raw number to degrees
    case RAW_TO_DEG:
      return (num / resolution) * 360;

    // Convert raw number to radians
    case RAW_TO_RAD:
      return (num / resolution) * 2 * M_PI;

    case DEG_TO_RAW: 
      return (num / 360) * resolution;

    // Convert from degrees to radians
    case DEG_TO_RAD:
      return num * M_PI / 180;

    // Convert from radians to degrees
    case RAD_TO_DEG:
      return num / M_PI * 180;
  }

  return num;
}

/**
 * Routine name: Encoder::~Encoder(void)
 * Description: Destructor for the encoder.
 * Parameters: None.
 */
Encoder::~Encoder(void) {}
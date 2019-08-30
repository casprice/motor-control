/**
 * File: Encoder.cpp
 * Description: Control class for the AS5048B magnetic encoder under an I2C bus.
 *              Reads the angle positions from the appropriate registers and
 *              converts the readings to degrees or radians.
 */

#include "Encoder.hpp"

/**
 * Routine name: Encoder::setZeroPosition(void)
 * Description: Set the zero position of the angle.
 * Parameters: None.
 * Return value: -1 if error occurred, 0 if successful.
 */
void Encoder::setZeroPosition(void) {
  calcRotation();
  zeroPosition = currAngle;
}

/**
 * Routine name: Encoder::calcRotation(void)
 * Description: Get angle by reading angle registers, which outputs values
 *              including zero position correction. Convert angle to degrees
 * 							according to its resolution.
 * Parameters: None.
 * Return value: None.
 */
int Encoder::calcRotation(void) {
  unsigned char * result;
  if ((result = BusDevice::readRegisters(ANGLMSB_REG)) == NULL) {
    cerr << "\nLost connection to the encoder. Aborting." << endl;
    return -1;
  }

  // Update previous angle
  prevAngle = currAngle;

  // Convert new reading to decimal and degrees, mod by 360
  currAngle = fmod((convertNum(toDecimal(result), RAW_TO_DEG)) - zeroPosition, 360);

  // Account for overflow
  currAngle = currAngle < 0 ? currAngle + 360 : currAngle;

  // Set new angle within the bounds of -180 to 180 degrees
  currAngle -= 180;

  delete[] result;

  return 0;
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
 * Routine name: Encoder::getAngle(void)
 * Description: Returns the current angle position in degrees.
 * Parameters: None.
 * Return value: The angle position.
 */
double Encoder::getAngle(void) {
  return currAngle;
}

/**
 * Routine name: Encoder::getVelocity(void)
 * Description: Returns the current velocity of the motor in degrees/second.
 * Parameters: None.
 * Return value: The angle position.
 */
double Encoder::getVelocity(void) {
  return (currAngle - prevAngle) / DT;
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

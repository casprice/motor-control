/**
 * File: Encoder.cpp
 * 
 * Description: TODO
 */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>				 
#include <fcntl.h>				 
#include <sys/ioctl.h>		 
#include <linux/i2c-dev.h> 

#include "Encoder.hpp"

/**
 * Routine name: Encoder::Encoder(unsigned int bus, unsigned int address) (public)
 * 
 * Description: Default constructor for Encoder. Initializes the Encoder values to 
 * zero. 
 */
Encoder::Encoder(unsigned int a_bus, unsigned int a_address) 
				: I2CDevice(a_bus, a_address) {
	bus = a_bus;
	address = a_address;
	angle = 0;
	memset(buffer, 0, sizeof(unsigned char));
	writeRegister(buffer, ANGLLSB_REG, ANGLMSB_REG);
	setZeroPosition();
}

/**
 * Routine name: Encoder::setZeroPosition(void) (public)
 * 
 * Description: Set the zero position of the angle using one time programmable
 *              (OTP) fuses for permanet programming of the user settings.
 * 
 * Parameters: None.
 * 
 * Return value: -1 if error occurred, 0 if successful.
 */
int Encoder::setZeroPosition(void) {
	// Write 0 int OTP zero position register to clear
	memset(buffer, 0, sizeof(unsigned char));
	if(writeRegister(buffer, ZEROLSB_REG, ZEROMSB_REG) == -1) {
		perror("Encoder: Could not write zero position.\n");
		return -1;
	}

	// Read angle information
	if(readRegister(buffer) == -1) {
		perror("Encoder: Could not read zero position.\n");
		return -1;
	}

	// Write previous read angle position into OTP zero position register
	if(writeRegister(buffer, ZEROLSB_REG, ZEROMSB_REG) == -1) {
		perror("Encoder: Could not write zero position.\n");
		return -1;
	}

	return 0;
}

/**
 * Routine name:
 * 
 * Description: Get angle by reading angle registers, which outputs values
 *              including zero position correction. Convert angle to degrees
 * 							according to its resolution.
 * 
 * Parameters:
 * 
 * Return value:
 */
double Encoder::calcRotation(double resolution) {
	//double rotation;

	writeRegister(buffer, ANGLLSB_REG, ANGLMSB_REG);
	readRegister(buffer);

	angle = toDegree(resolution, (double)toDecimal(buffer));

	//angle = (double)rotation - zeroPosition;
	
	return 1;
}

/**
 * Routine name:
 * 
 * Description: Returns the current zero position.
 * 
 * Parameters:
 */
double Encoder::getAngle(void) {
	return angle;
}

/**
 * Routine names:
 * 
 * Description: Combines two 8-bit registers into a 16-bit short. 
 * 
 * Parameters:
 */
short Encoder::toDecimal(unsigned char * buf) {
  // (i2c_device.getAngle()[0] << 6) + (i2c_device.getAngle()[1] & 0x3F)
	// (( ( left_byte & 0xFF ) << 8 ) | ( right_byte & 0xFF )) & ~0xC000;
  //return (buf[1] << 8) + (buf[0] & 0xFF);
	// (buf[1] << 6) + (buf[0] & 0x3F)

	// buf[0] uses 6 bits and buf[1] uses 8 bits.
  return ((short)buf[1] << 6) | ((short)buf[0] & 0x3F);
}

/**
 * Routine name:
 * 
 * Description: Convert num into degrees using given resolution.
 * 
 * Parameters:
 */
double Encoder::toDegree(double resolution, double num) {
  return (num / resolution) * NUM_DEG;
}

/**
 * Routine name:
 * 
 * Description:
 */
Encoder::~Encoder(void) {}
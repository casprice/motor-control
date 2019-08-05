#include "I2CDevice.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>				  // Needed for I2C port
#include <sys/ioctl.h>			// Needed for I2C port
#include <linux/i2c-dev.h>  // Needed for I2C port
using namespace std;

/**
 * Constructor for the I2CDevice class. It requires the bus number and device number. The constructor
 * opens a file handle to the I2C device, which is destroyed when the destructor is called
 * @param bus The bus number. Usually 0 or 1 on the BBB
 * @param device The device ID on the bus.
 */
I2CDevice::I2CDevice(unsigned int a_bus, unsigned int a_address) {
	bus = a_bus;
	address = a_address;
	file=-1;
	openDevice();
}

/**
 * Open a connection to an I2C device
 * @return -1 on failure to open to the bus or device, 0 on success.
 */
int I2CDevice::openDevice(void){
  string name = BBB_I2C_2;
  // Choose correct bus
  switch(address) {
    case 0:
      name = BBB_I2C_0;
      break;
    case 1:
      name = BBB_I2C_1;
      break;
    case 2:
      name = BBB_I2C_2;
      break;
  }

  // Open the I2C bus
  if((file = open(name.c_str(), O_RDWR)) < 0){
    perror("I2C: failed to open the bus\n");
    return -1;
  }
  if(ioctl(file, I2C_SLAVE, address) < 0){
    perror("I2C: Failed to connect to the device\n");
    return -1;
  }
  
  return 0;
}

/**
 * Write a single byte value to a single register. returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
 * @param registerAddress The register address
 * @param value The value to be written to the register
 * @return -1 on failure to write, 0 on success.
 */
int I2CDevice::writeRegister(unsigned char * buffer, unsigned int lsb, unsigned int msb){
  buffer[0] = lsb;
  //buffer[1] = msb;

  if (write(file, buffer, 1) != 1) {
    perror("I2C: Failed to write to the i2c bus.\n");
    return -1;
  }
  return 0;
}

/**
 * Read a single register value from the address on the device. returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
 * @param registerAddress the address to read from
 * @return the byte value at the register address.
 */
int I2CDevice::readRegister(unsigned char * buffer){
  if (read(file, buffer, 2) != 2)	{
    perror("I2C: Failed to read from the i2c bus.\n");
    return -1;
  }

  return 0;
}

/**
 * Close the file handles and sets a temporary state to -1.
 */
void I2CDevice::closeDevice(void){
	close(file);
	file = -1;
}

/**
 * Closes the file on destruction, provided that it has not already been closed.
 */
I2CDevice::~I2CDevice(void) {
	if(file!=-1) {
    close(file);
  }
}
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
 * Write a single value to the I2C device. Used to set up the device to read from a
 * particular address.
 * @param value the value to write to the device
 * @return 1 on failure to write, 0 on success.
 */
int I2CDevice::writeRegister(unsigned char value) {
   unsigned char buffer[1];
   buffer[0]=value;
   if (write(file, buffer, 1) != 1){
      perror("I2C: Failed to write to the device\n");
      return 1;
   }
   return 0;
}

/**
 * Method to read a number of registers from a single device. This is much more efficient than
 * reading the registers individually. The from address is the starting address to read from, which
 * defaults to 0x00.
 * @param number the number of registers to read from the device
 * @param fromAddress the starting address to read from
 * @return a pointer of type unsigned char* that points to the first element in the block of registers
 */
unsigned char * I2CDevice::readRegisters(unsigned int number, unsigned int fromAddress){
	unsigned char* data = new unsigned char[number];
  writeRegister(fromAddress);
  if (read(file, data, number) != (int)number) {
    perror("I2C: Failed to read in the full buffer.\n");
    return NULL;
  }
	return data;
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
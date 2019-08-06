#include "I2CDevice.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
using namespace std;

/**
 * Routine name: I2CDevice::I2CDevice(unsigned int a_bus, unsigned int a_address)
 * Description: Constructor for the I2CDevice class. It requires the bus number
 *              and device number. Opens a file handle to the I2C device, which
 *              is destroyed when the destructor is called
 * Parameters: bus - The bus number. Usually bus #2
 *             device - The device ID on the bus.
 */
I2CDevice::I2CDevice(unsigned int a_bus, unsigned int a_address) {
	bus = a_bus;
	address = a_address;
	file=-1;
	openDevice();
}

/**
 * Routine name: I2CDevice::openDevice(void)
 * Description: Opens a connection to the device through an I2C bus.
 * Parameters: None.
 * Return value: -1 on failure to open to the bus or device, 0 on success.
 */
int I2CDevice::openDevice(void) {
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

  // Initiate communication with the device
  if(ioctl(file, I2C_SLAVE, address) < 0){
    perror("I2C: Failed to connect to the device\n");
    return -1;
  }
  
  return 0;
}

/**
 * Routine name: I2CDevice::writeRegister(unsigned char value)
 * Description: Writes a value to the I2C device. Used to set up the 
 *              device to read from a particular address.
 * Parameters: value - the value to write to the device
 * Return value: 1 on failure to write, 0 on success.
 */
int I2CDevice::writeRegister(unsigned char value) {
  unsigned char buffer[1];
  buffer[0] = value;
  
  if (write(file, buffer, 1) != 1){
    perror("I2C: Failed to write to the device\n");
    return 1;
  }

  return 0;
}

/**
 * Routine name: I2CDevice::readRegisters(unsigned int number, unsigned int fromAddress)
 * Description: Reads number of registers from a device. 
 * Parameters: number - the number of registers to read from the device
 *             fromAddress - the starting address to read from
 * Return value: a pointer of type unsigned char * that points to the first
 *               element in the block of registers.
 */
unsigned char * I2CDevice::readRegisters(unsigned int number, unsigned int fromAddress) {
	unsigned char* data = new unsigned char[number];
  writeRegister(fromAddress);
  if (read(file, data, number) != (int)number) {
    perror("I2C: Failed to read in the full buffer.\n");
    return NULL;
  }
	return data;
}

/**
 * Routine name: I2CDevice::closeDevice(void)
 * Description: Close the file handles and sets a temporary state to -1.
 * Parameters: None.
 * Return value: None.
 */
void I2CDevice::closeDevice(void) {
	close(file);
	file = -1;
}

/**
 * Routine name: I2CDevice::~I2CDevice(void)
 * Description: Closes the file on destruction, provided that it has not 
 *              already been closed.
 * Parameters: None.
 */
I2CDevice::~I2CDevice(void) {
	if (file != -1) {
    close(file);
  }
}
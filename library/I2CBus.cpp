#include "I2CBus.h"

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
using namespace std;

// Global static pointer used to ensure a single instance of the class.
busTracker* busTracker::theTracker = nullptr;

/**
 * Routine name: I2CBus(unsigned int a_bus)
 * Description: Constructor for the I2CBus class. It requires the bus number
 *              and device number. Opens a file handle to the I2C device, which
 *              is destroyed when the destructor is called. Can only be called
 *              by the getInstance() function.
 * Parameters: bus - The bus number. Usually bus #2
 *             device - The device ID on the bus.
 */
I2CBus::I2CBus(unsigned int a_bus) {
	bus = a_bus;
	file=-1;
  busTracker* theTracker = busTracker::getInstance();
	openDevice();
}

/**
 * Routine name: openDevice(void)
 * Description: Opens a connection to the device through an I2C bus.
 * Parameters: None.
 * Return value: -1 on failure to open to the bus or device, 0 on success.
 */
int I2CBus::openDevice(void) {
  string name = BBB_I2C_2;
  // Choose correct bus
  switch(bus) {
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
    perror("\n\rI2C: failed to open the bus");
    return -1;
  }

  // Initiate communication with the device
  /*
  if(ioctl(file, I2C_SLAVE, address) < 0) {
    perror("\n\rI2C: Failed to connect to the device");
    return -1;
  }*/

  for(int addr = 1; addr < 127; addr++) {
    ioctl(file, I2C_SLAVE, addr);
  }
  
  return 0;
}

/**
 * Routine name: writeRegister(unsigned char value)
 * Description: Writes a value to the I2C device. Used to set up the 
 *              device to read from a particular address.
 * Parameters: value - the value to write to the device
 * Return value: 1 on failure to write, 0 on success.
 */
int I2CBus::writeRegister(unsigned char value) {
  unsigned char buffer[1];
  buffer[0] = value;
  
  if (write(file, buffer, 1) != 1) {
    perror("\n\rI2C: Failed to write to the device");
    return -1;
  }

  return 0;
}

/**
 * Routine name: readRegisters(unsigned int fromAddress)
 * Description: Reads number of registers from a device. 
 * Parameters: fromAddress - the starting address to read from
 * Return value: a pointer of type unsigned char * that points to the first
 *               element in the block of registers.
 */
unsigned char * I2CBus::readRegisters(unsigned int fromAddress) {
	unsigned char* data = new unsigned char[2];
  if(writeRegister(fromAddress) == -1) {
    return NULL;
  }
  if (read(file, data, 2) != 2) {
    perror("\n\rI2C: Failed to read in the full buffer.");
    return NULL;
  }
	return data;
}

/**
 * Routine name: closeDevice(void)
 * Description: Close the file handles and sets a temporary state to -1.
 * Parameters: None.
 * Return value: None.
 */
void I2CBus::closeDevice(void) {
	close(file);
	file = -1;
}

/**
 * Routine name: ~I2CBus(void)
 * Description: Closes the file on destruction, provided that it has not 
 *              already been closed.
 * Parameters: None.
 */
I2CBus::~I2CBus(void) {
	if (file != -1) {
    close(file);
  }
}
#ifndef I2C_H_
#define I2C_H_

#define BBB_I2C_0 "/dev/i2c-0"
#define BBB_I2C_1 "/dev/i2c-1"
#define BBB_I2C_2 "/dev/i2c-2"

/**
 * @class I2CDevice
 * @brief Generic I2C Device class that can be used to connect to any type of I2C device and read or write to its registers
 */
class I2CDevice {
protected:
	unsigned int bus;     // the bus number
	unsigned int address; // the device number on the bus
	int file;             // the file handle to the device

public:
	I2CDevice(unsigned int a_bus, unsigned int a_device);
	int openDevice(void);
	int writeRegister(unsigned char * buffer, unsigned int lsb, unsigned int msb);
	int readRegister(unsigned char * buffer);
	void closeDevice(void);
	~I2CDevice(void);
};

#include "I2CDevice.cpp"

#endif /* I2C_H_ */
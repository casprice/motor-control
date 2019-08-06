#ifndef I2C_H_
#define I2C_H_

#define BBB_I2C_0 "/dev/i2c-0"
#define BBB_I2C_1 "/dev/i2c-1"
#define BBB_I2C_2 "/dev/i2c-2"

class I2CBus {
	protected:
		unsigned int bus;     // the bus number
		unsigned int address; // the device number on the bus
		int file;             // the file handle to the device
		
		// Protected constructor to prevent public call
		I2CBus(unsigned int a_bus, unsigned int a_device);

	public:
		static I2CBus* getInstance(unsigned int a_bus, unsigned int a_address);
		int openDevice(void);
		int writeRegister(unsigned char value);
		unsigned char* readRegisters(unsigned int number, unsigned int fromAddress);
		void closeDevice(void);
		~I2CBus(void);
};

#include "I2CBus.cpp"

#endif /* I2C_H_ */
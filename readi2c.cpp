#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <i2c/smbus.h>

#define I2C_SLAVE	0x0703	/* Use this slave address */

/*
 * Read a register from the sensor
 * Takes the address of the register as a 16 bit short
 * Returns the value of the register
 */
short Encoder::read(short registerAddress){
	short command = 0b0100000000000000; // PAR=0 R/W=R
	command = command | registerAddress;

	//Add a parity bit on the the MSB
	command |= ((short)calcEvenParity(command)<<15);

	//Split the command into two bytes
	char right_byte = command & 0xFF;
	char left_byte = ( command >> 8 ) & 0xFF;

	//Return the data, stripping the parity and error bits
	return (( ( left_byte & 0xFF ) << 8 ) | ( right_byte & 0xFF )) & ~0xC000;
}

/*
 * Write to a register
 * Takes the 16-bit  address of the target register and the 16 bit short of data
 * to be written to that register
 * Returns the value of the register after the write has been performed. This
 * is read back from the sensor to ensure a sucessful write.
 */
short Encoder::write(short registerAddress, short data) {

	short command = 0b0000000000000000; // PAR=0 R/W=W
	command |= registerAddress;

	//Add a parity bit on the the MSB
	command |= ((short)calcEvenParity(command)<<15);

	//Split the command into two bytes
	char right_byte = command & 0xFF;
	char left_byte = ( command >> 8 ) & 0xFF;
	
	short dataToSend = 0b0000000000000000;
	dataToSend |= data;

	//Craft another packet including the data and parity
	dataToSend |= ((short)calcEvenParity(dataToSend)<<15);
	right_byte = dataToSend & 0xFF;
	left_byte = ( dataToSend >> 8 ) & 0xFF;

	//Return the data, stripping the parity and error bits
	return (( ( left_byte & 0xFF ) << 8 ) | ( right_byte & 0xFF )) & ~0xC000;
}

int main(void) {
	int file_i2c;
	int length;
	unsigned char buffer[60] = {0};

	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-2";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	int addr = 0x5a;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}
	
	
	//----- READ BYTES -----
	length = 4;			//<<< Number of bytes to read
	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}

	
	//----- WRITE BYTES -----
	buffer[0] = 0x01;
	buffer[1] = 0x02;
	length = 2;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
}
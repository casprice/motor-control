/**
 * File: Encoder.cpp
 */
#include <stdlib.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

#include "Encoder.hpp"
#include "Driver.hpp"
#include "library/gpio.h"
#include "library/I2CDevice.h"
#include "library/pwm.h"

#define NUM_BYTES 4 // number of bytes to read
#define ANGLMSB_REG 0xFE // angle msb
#define ANGLLSB_REG 0xFF // angle lsb
#define MAGNMSB_REG 0xFC // magnitude msb
#define MAGNLSB_REG 0xFD // magnitude lsb
#define RESOLUTION 16384.0
#define NUM_DEG 360

int address;
int busNum;
double angleZero;
double magnitudeZero;
int file_i2c;
int length;
unsigned char buffer[60] = {0};

// Default ctor
Encoder::Encoder() {
    address = 0x40;
    busNum = 2;
    angleZero = 0;
    magnitudeZero = 0;
}

// Copy ctor
Encoder::Encoder(int a_address, int a_busNum) {
    address = a_address;
    busNum = a_busNum;
    angleZero = 0;
    magnitudeZero = 0;
}

Encoder::~Encoder() {}
/* 
void Encoder::zeroAngle() {
    exploringBB::I2CDevice i2c(busNum, address);
    i2c.open();
    angleZero = *(double *)(i2c.readRegisters(8, ANGLMSB_REG));
    i2c.close();
}*/

double Encoder::getAngle() {
    double val = 0;
    /*
    exploringBB::I2CDevice i2c(busNum, address);
    i2c.open();
    val = *(double *)(i2c.readRegisters(4, ANGLMSB_REG));
    i2c.close();*/
    return val;
}
/* 
void Encoder::zeroMagnitude() {
    exploringBB::I2CDevice i2c(busNum, address);
    i2c.open();
    magnitudeZero = *(double *)(i2c.readRegisters(8, MAGNMSB_REG));
    i2c.close();
}

double Encoder::getMagnitude() {
    double val = 0;
    exploringBB::I2CDevice i2c(busNum, address);
    i2c.open();
    val = *(double *)(i2c.readRegisters(2, MAGNMSB_REG));
    i2c.close();
    return val;
}*/

double Encoder::toDegree(double num) {
    return (num / RESOLUTION) * NUM_DEG;
}

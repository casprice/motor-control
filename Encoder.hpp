/**
 * File: Encoder.hpp
 */

#include "library/I2CBus.h"
#include "library/BusDevice.h"

#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
using namespace std;

#ifndef ENCODER_HPP
#define ENCODER_HPP

// Default values
#define DEFAULT_BUS 2        // default i2c bus to read from
#define DEFAULT_ADDRESS 0x40 // default address of the device

// Constants for an AS5048 magnetic encoder
#define ANGLMSB_REG 0xFE     // bits 0..7
#define ANGLLSB_REG 0xFF     // bits 0..5
#define RESOLUTION 16384.0   // 14 bits

// Unit constants
#define RAW_TO_DEG 1
#define RAW_TO_RAD 2
#define DEG_TO_RAD 3
#define RAD_TO_DEG 4
#define RANGE_VAL 180

class Encoder : public BusDevice {
  private:
    double zeroPosition = 0;
    double prevAngle = 0;
    double currAngle = 0;
    double resolution;
    int dt;

  public:
    /**
     * Routine name: Encoder::Encoder(unsigned int bus, unsigned int address)
     * Description: Default constructor for Encoder. Sets up an instance of the I2C bus, 
     *              if one does not exist and initializes bus number, device address,
     *              and resolution. Sets the initial position of the motor as the zero
     *              position.
     * Parameters: a_bus - the bus number of the device
     *             a_address - the address ID
     *             a_resolution - the 14-bit resolution
     */
    Encoder(I2CBus* a_bus,
            int a_dt,
            unsigned int a_address, 
            double a_resolution=RESOLUTION) 
    : BusDevice(a_bus, a_address) {
      resolution = a_resolution;
      dt = a_dt;
      cout << "Initialized encoder at address " << hex << a_address << endl;
      setZeroPosition();
    }

    void setZeroPosition(void);
    int calcRotation(void);
    double getZero(void);
    double getAngle(void);
    double getVelocity(void);
    short toDecimal(unsigned char* buf);
    double convertNum(double num, int conversion);
    
    ~Encoder(void);
};

#include "Encoder.cpp"

#endif

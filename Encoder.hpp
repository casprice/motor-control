/**
 * File: Encoder.hpp
 */

#include "library/I2CBus.h"

#ifndef ENCODER_HPP
#define ENCODER_HPP

#define DEFAULT_BUS 2        // default i2c bus to read from
#define DEFAULT_ADDRESS 0x40 // default address of the device
#define ANGLMSB_REG 0xFE     // bits 0..7
#define ANGLLSB_REG 0xFF     // bits 0..5
#define RESOLUTION 16384.0   // 14 bits
#define DT 0.1

class Encoder {
  private:
    double zeroPosition = 0;
    double prevAngle = 0;
    double currAngle = 0;
    double resolution;

    I2CBus *busTracker;

  public:
    Encoder(unsigned int a_bus=DEFAULT_BUS, 
            unsigned int a_address=DEFAULT_ADDRESS, 
            double a_resolution=RESOLUTION);

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
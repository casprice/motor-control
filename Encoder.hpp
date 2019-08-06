/**
 * File: Encoder.hpp
 */

#include "library/I2CDevice.h"

#ifndef ENCODER_HPP
#define ENCODER_HPP

#define DEFAULT_BUS 2      // default 12c bus to read from
#define ADDRESS 0x40       // address of the device
#define ZEROMSB_REG 0x16
#define ZEROLSB_REG 0x17
#define ANGLMSB_REG 0xFE   // bits 0..7
#define ANGLLSB_REG 0xFF   // bits 0..5
#define RESOLUTION 16384.0 // 14 bits
#define NUM_DEG 360

class Encoder : protected I2CDevice {
    private:
        double angle;
        double resolution;
        double zeroPosition;
        unsigned char buffer[60];

    public:
        Encoder(unsigned int a_bus=DEFAULT_BUS, unsigned int a_address=ADDRESS, double a_resolution=RESOLUTION);

        int setZeroPosition(void);
        double getAngle(void);
        double getZero(void);
        double calcRotation();
        short toDecimal(unsigned char * buf);
        double toDegree(double num);
        
        ~Encoder(void);
};

#include "Encoder.cpp"

#endif
/**
 * File: Encoder.hpp
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP

class Encoder {
    private:
        int address;
        int busNum;
        uint8_t angleZero;
        uint8_t magnitudeZero;

    public:
        Encoder();
        Encoder(int address, int busNum);
        ~Encoder();

        void zeroAngle();
        double getAngle();
        void zeroMagnitude();
        double getMagnitude();
        double toDegree(double num);
};

#include "Encoder.cpp"

#endif
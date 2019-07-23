/**
 * File: PID.hpp
 */

#ifndef PID_HPP
#define PID_HPP

class PID {
    private:
        double Kp;
        double Ki;
        double Kd;
        double totalError;
        double prevError;

    public:
        PID();
        PID(double Kp, double Ki, double Kd);
        ~PID();

        void update(double current, int goal, bool invertDir);
        void clearKi();
}

#include "PID.cpp"

#endif
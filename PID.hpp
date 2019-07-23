#ifndef PID_HPP
#define PID_HPP

using namespace std;

class PID {
    private:
        double Kp;
        double Ki;
        double Kd;

    public:
        PID();
        ~PID();

        void init(double Kp, double Ki, double Kd);
        void update(double current, int goal, bool invertDir);
        void clearKi();
}

#include "PID.cpp"

#endif
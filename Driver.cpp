/**
 * File: Driver.cpp
 */
#include <signal.h>
#include <rc/time.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <curses.h>

#include "Driver.hpp"
#include "Encoder.hpp"
#include "PID.hpp"
#include "library/gpio.h"
#include "library/pwm.h"
#include "library/util.h"
using namespace std;

#define FREQ 20000 // 20 kHz
#define STEP 10
#define MIN_ANGLE -175
#define MAX_ANGLE 175

static int running = 0;

/**
 * Handle Ctrl-C interrupt
 */
static void __signal_handler(__attribute__ ((unused)) int dummy) {
    running = 0;
    return;
}

/**
 * Ensure number doesn't exceed min or max angles
 */
double clip(double number, int min, int max) {
    if (number < min) number = min;
    if (number > max) number = max;
    return number;
}

/**
 * Main driver of the motor control for the snake.
 */
int main() {
    // Register keyboard interrupt
    signal(SIGINT, __signal_handler);
    running = 1;

    // Curses initialization
    initscr(); // start curses
    cbreak();  // react to keys instantly w/o Enter
    noecho();  // don't echo input to screen
    keypad(stdscr, TRUE); // register arrow keys

    int value = 0;   // angle we want motor to spin to
    int counter = 0; // counter for Ki

    // GPIO/PWM setup
    PWM pwm("P9_14");
    GPIO dir(DIR_PIN);
    GPIO enable(ENABLE_PIN);
    GPIO pwm_pin(PWM_PIN);
    //system("echo out > /sys/class/gpio/gpio50/direction");
    //system("echo 1 > /sys/class/gpio/gpio50/value");
    dir.exportGPIO();
    dir.setDirection(GPIO::OUTPUT);
    dir.setValue(GPIO::HIGH);
    enable.exportGPIO();
    enable.setDirection(GPIO::OUTPUT);
    enable.setValue(GPIO::HIGH);
    pwm_pin.exportGPIO();
    pwm_pin.setDirection(GPIO::OUTPUT);
    pwm_pin.setValue(GPIO::HIGH);
    pwm.setFrequency(20000);

    // Create new PID and Encoder controllers
    PID pidCtrl(0.8, 0.0003, 0.008);
    Encoder i2cDevice;
    
    mvaddstr(0, 1, "Angle: 0");
    mvaddstr(1, 1, "Position: -180");

    // main loop
    while (running) {
        int ch = getch();
        switch (ch) {
            case KEY_LEFT:
                value -= STEP;
                break;
            case KEY_RIGHT:
                value += STEP;
                break;
            case 'q':
                running = 0;
            default:
                continue;
        }

        value = clip(value, MIN_ANGLE, MAX_ANGLE);
        refresh(); // clear the terminal window
        //clear();

        stringstream ss1;
        ss1 << "Angle: " << value;
        const char * str1 = ss1.str().c_str();
        mvaddstr(0, 1, str1);
 
        double angle = i2cDevice.getAngle();
        angle = i2cDevice.toDegree(angle)-180;
        
        stringstream ss2;
        ss2 << "Position: " << angle;
        const char * str2 = ss2.str().c_str();
        mvaddstr(1, 1, str2);

        pidCtrl.update(dir, pwm, angle, value, false);

        counter++;
        if ((counter % 1000) == 0) pidCtrl.clearKi();

        //rc_usleep(1000);
    }

    enable.setValue(GPIO::LOW);
    dir.setValue(GPIO::LOW);
    pwm_pin.setValue(GPIO::LOW);

    endwin();  // restore terminal from curses

    return 0;
}
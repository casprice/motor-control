/**
 * File: Driver.cpp
 */
#include <rc/gpio.h>
#include <rc/pwm.h>
#include <signal.h>
#include <rc/time.h>

#include "Driver.hpp"
#include "Encoder.hpp"
#include "PID.hpp"
using namespace std;

#define FREQ 20000 // 20 kHz
#define COARSE_STEP 10
#define FINE_STEP 1
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
    int step = COARSE_STEP;   // how much to increase value

    // GPIO/PWM setup
    rc_gpio_init(DIR_CHIP, DIR_PIN, GPIOHANDLE_REQUEST_OUTPUT);
    rc_gpio_init(ENABLE_CHIP, ENABLE_PIN, GPIOHANDLE_REQUEST_OUTPUT);
    rc_pwm_init(PWM_SUBSYS, FREQ);
    system("echo out > /sys/class/gpio/gpio" + PWM_PIN + "/direction");
    system("echo 1 > /sys/class/gpio/gpio" + PWM_PIN + "/value");
    rc_gpio_set_value(ENABLE_CHIP, ENABLE_PIN, HIGH);
    rc_pwm_set_duty(PWM_SUBSYS, PWM_CH_A, 0);
    rc_pwm_set_duty(PWM_SUBSYS, PWM_CH_B, 0);

    // Create new PID and Encoder controllers
    PID pidCtrl = new PID(0.8, 0.0003, 0.008);
    Encoder i2cDevice = new Encoder();

    // main loop
    while (running) {
        int ch = getch();
        switch (ch) {
            case KEY_LEFT:
                value -= step;
            case KEY_RIGHT:
                value += step;
            case ord(' '):
                if(step == COARSE_STEP) step = FINE_STEP;
                else step = COARSE_STEP;
        }

        value = clip(value, MIN_ANGLE, MAX_ANGLE);
        refresh(); // clear the terminal window
        mvaddstr(1, 1, "Angle: %0.3f", value);

        double angle = i2cDevice.getAngle();
        mvaddstr(1, 1, "Position: %0.3f", i2cDevice.toDegree(angle)-180);

        pidCtrl.update(i2cDevice.toDegree(angle)-180, value, false);

        counter++;
        if ((counter % 1000) == 0) pidCtrl.clearKi();

        rc_usleep(1000);
    }

    rc_gpio_set_value(ENABLE_CHIP, ENABLE_PIN, LOW);

    // Cleanup
    rc_gpio_cleanup(DIR_CHIP, DIR_PIN);
    rc_gpio_cleanup(ENABLE_CHIP, ENABLE_PIN);
    rc_pwm_cleanup(PWM_SUBSYS);
    endwin();  // restore terminal from curses

    return 0;
}
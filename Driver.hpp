#ifndef DRIVER_H
#define DRIVER_H

#define DIR_CHIP 0
#define DIR_PIN 48    // P9_15
#define ENABLE_CHIP 0
#define ENABLE_PIN 60 // P9_12
#define PWM_PIN 50    // P9_14
#define PWM_SUBSYS 1
#define PWM_CH_A 'A'
#define PWM_CH_B 'B'
#define HIGH 1
#define LOW 0

double clip(double number, int min, int max);

#endif
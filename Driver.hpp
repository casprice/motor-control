#ifndef DRIVER_H
#define DRIVER_H

#define M1_DIR 48    // P9_15
#define M1_ENABLE 60 // P9_12

#define M2_DIR 45    // P8_10
#define M2_ENABLE 68 // P8_11

#define M3_DIR 80    // P8_36
#define M3_ENABLE 8  // P8_35

//#define PWM_PIN 50   // P9_14

#define MOTOR1_ADC 1  // AIN1
#define MOTOR2_ADC 3  // AIN3
#define MOTOR3_ADC 5  // AIN5

double clip(double number, int min, int max);

#endif
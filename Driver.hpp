#ifndef DRIVER_H
#define DRIVER_H

#define M1_PWM 1     // P8_13
#define M1_DIR 48    // P9_15
#define M1_ENABLE 60 // P9_12

#define M2_PWM 2     // P8_19
#define M2_DIR 45    // P8_10
#define M2_ENABLE 68 // P8_11

#define M3_PWM 3     // P9_16
#define M3_DIR 80    // P8_36
#define M3_ENABLE 8  // P8_35

// P8_13, P8_19, P9_16
int pwm_ss[] = { 2,   2,   1 };  // subsystem
int pwm_ch[] = {'B', 'A', 'B'};  // channel

// P9_15, P8_10, P8_36
int direction_vals[] = { 48, 45, 80 };

// P9_12, P8_11, P8_35
int enable_vals[] = { 60, 68, 8 };

// AIN1, AIN3, AIN5
int motor_adc[] = { 1, 3, 5 };

// Encoder addresses
unsigned int enc_addr[] = { 0x40, 0x41, 0x42 };

#endif
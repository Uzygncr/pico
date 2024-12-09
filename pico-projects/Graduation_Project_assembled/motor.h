#ifndef MOTOR_H
#define MOTOR_H

#include "pico/stdlib.h"

// Motor Tanımları
#define STEP_PIN 15
#define DIR_PIN 14
#define MAX_SPEED 20000
#define MIN_SPEED 750
#define MAX_ACCELERATION 200000
#define MAX_JERK 2000000
#define MIN_ACCEL_DISTANCE_RATIO 0.20
#define MAX_ACCEL_DISTANCE_RATIO 0.30

// Fonksiyon Prototipleri
void motor_init();
void step_motor(int steps, bool direction);
int get_position();
float get_speed();
float get_acceleration();

#endif // MOTOR_H

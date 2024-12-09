#include "motor.h"
#include <math.h>

// Global pozisyon değişkeni
static int position = 0;
static float motor_speed = 0.0;         // Motorun mevcut hızı (adım/s)
static float motor_acceleration = 0.0; // Motorun mevcut ivmesi (adım/s²)
static absolute_time_t last_update_time; // Son hız/ivme güncelleme zamanı

// Motor GPIO pinlerini başlatır
void motor_init() {
    gpio_init(STEP_PIN);
    gpio_set_dir(STEP_PIN, GPIO_OUT);
    gpio_init(DIR_PIN);
    gpio_set_dir(DIR_PIN, GPIO_OUT);

    // İlk güncelleme zamanını ayarla
    last_update_time = get_absolute_time();
}

// Motorun pozisyonunu döndürür
int get_position() {
    return position;
}

// Motorun hızını döndürür
float get_speed() {
    return motor_speed;
}

// Motorun ivmesini döndürür
float get_acceleration() {
    return motor_acceleration;
}

// Motorun hız ve ivme değerlerini günceller
static void update_motion_data(int steps) {
    // Şu anki zaman
    absolute_time_t now = get_absolute_time();
    int64_t delta_time_us = absolute_time_diff_us(last_update_time, now);

    // Zaman farkı (saniye cinsinden)
    float delta_time_s = delta_time_us / 1000000.0;

    // Hız hesaplama
    float delta_position = (float)steps; // Hareket edilen adım sayısı
    float new_speed = delta_position / delta_time_s;

    // İvme hesaplama
    motor_acceleration = (new_speed - motor_speed) / delta_time_s;

    // Güncel hız
    motor_speed = new_speed;

    // Zamanı güncelle
    last_update_time = now;
}

// Motoru çalıştırır
void step_motor(int steps, bool direction) {
    gpio_put(DIR_PIN, direction); // Motor yönünü ayarla

    // Hareket algoritması
    float current_speed = MIN_SPEED;
    float acceleration = 0;
    float jerk = 0;
    float obtained_speed = MAX_SPEED;

    float accel_distance = steps * MAX_ACCEL_DISTANCE_RATIO;
    if (steps < 2000) {
        accel_distance = steps * MIN_ACCEL_DISTANCE_RATIO;
    }

    float decel_start = steps - accel_distance;
    float accel_time = cbrtf((6.0f * accel_distance) / MAX_JERK);
    float time_step = accel_time / accel_distance;

    for (int i = 0; i < steps; i++) {
        int delay_us = 1000000 / current_speed;

        // Adım sinyali gönder
        gpio_put(STEP_PIN, true);
        sleep_us(10); // Pulse genişliği
        gpio_put(STEP_PIN, false);
        sleep_us(delay_us - 10);

        // Pozisyon güncelle
        if (direction) {
            position++;
        } else {
            position--;
        }

        // Hız ve ivme güncellemesi
        if (i < accel_distance) {
            if (i < accel_distance / 2) {
                jerk = MAX_JERK;
                acceleration += jerk * time_step;
                if (acceleration > MAX_ACCELERATION) acceleration = MAX_ACCELERATION;
            } else {
                jerk = -MAX_JERK;
                acceleration += jerk * time_step;
                if (acceleration < 0) acceleration = 0;
            }
            current_speed += acceleration * time_step;
            if (current_speed > obtained_speed) current_speed = obtained_speed;
        } else if (i >= decel_start) {
            if (i < decel_start + accel_distance / 2) {
                jerk = MAX_JERK;
                acceleration += jerk * time_step;
                if (acceleration > MAX_ACCELERATION) acceleration = MAX_ACCELERATION;
            } else {
                jerk = -MAX_JERK;
                acceleration += jerk * time_step;
                if (acceleration < 0) acceleration = 0;
            }
            current_speed -= acceleration * time_step;
            if (current_speed < MIN_SPEED) current_speed = MIN_SPEED;
        }
    }

    // Hareketin sonunda hız ve ivme güncellenir
    update_motion_data(steps);
}

#include "pico/stdlib.h"
#include "motor.h"
#include "encoder.h"
#include "lcd.h"
#include "blink.h"
#include "calibration.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Matematiksel hesaplamalar için

#define RADIUS_MM 330.0 // Çubuğun uzunluğu (mm)
#define ENCODER_TICKS_PER_REV 1000 // Encoder tam tur tık sayısı
#define DEGREE_PER_TICK (360.0 / ENCODER_TICKS_PER_REV)

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global değişkenler
volatile int latest_encoder_ticks = 0;
volatile float last_angle = 0.0;
volatile float angular_velocity = 0.0;
volatile float angular_acceleration = 0.0;
absolute_time_t last_update_time;

// Encoder kontrolü için 100 µs zamanlayıcı callback
bool update_encoder_callback(repeating_timer_t *rt) {
    latest_encoder_ticks = encoder_get_ticks();
    return true; // Timer devam etsin
}

// Açısal hız ve ivme hesaplama fonksiyonu
void calculate_angular_data() {
    // Şu anki zaman
    absolute_time_t now = get_absolute_time();
    int64_t delta_time_us = absolute_time_diff_us(last_update_time, now);
    float delta_time_s = delta_time_us / 1000000.0;

    // Mevcut açı
    float current_angle = latest_encoder_ticks * DEGREE_PER_TICK;

    // Açısal hız hesaplama
    float delta_angle = current_angle - last_angle;
    float new_angular_velocity = delta_angle / delta_time_s;

    // Açısal ivme hesaplama
    angular_acceleration = (new_angular_velocity - angular_velocity) / delta_time_s;

    // Güncelle
    angular_velocity = new_angular_velocity;
    last_angle = current_angle;
    last_update_time = now;
}

// Ark uzunluğu, hızı ve ivmesi ile açı, açısal hız ve ivmeyi hesaplama ve yazdırma
// Ark uzunluğu, hızı ve ivmesi ile açı, açısal hız ve ivmeyi hesaplama ve yazdırma
void print_data() {
    // Mevcut pozisyon, hız ve ivme değerlerini al
    float position = get_position();
    float speed = get_speed();
    float acceleration = get_acceleration();

    // Ark verilerini hesapla
    float position_rad = position * (M_PI / 180.0); // Dereceden radyana
    float speed_rad = speed * (M_PI / 180.0);       // Dereceden radyana
    float acceleration_rad = acceleration * (M_PI / 180.0); // Dereceden radyana

    float arc_length = position_rad * RADIUS_MM; // mm
    float arc_velocity = speed_rad * RADIUS_MM; // mm/s
    float arc_acceleration = acceleration_rad * RADIUS_MM; // mm/s²

    // Açısal verileri hesapla
    calculate_angular_data();

    // UART üzerinden sabit genişlikte yazdır
    printf("| %-15s | %-17s | %-20s |\n", "Değer", "Ark Verisi", "Açısal Verisi");
    printf("|----------------|-------------------|--------------------|\n");
    printf("| %-14s | %9.2f mm      | %9.2f derece   |\n", "Uzunluk", arc_length, last_angle);
    printf("| %-15s | %9.2f mm/s    | %9.2f derece/s |\n", "Hız", arc_velocity, angular_velocity);
    printf("| %-15s | %9.1f mm/s²   | %9.1f derece/s²|\n", "İvme", arc_acceleration, angular_acceleration);
    printf("\n"); // Boşluk bırak
    }
bool print_data_callback(repeating_timer_t *rt) {
    print_data();
    return true; // Timer devam etsin
}

int main() {
    stdio_init_all();
    printf("Sistem başlatılıyor...\n");

    // Motor, Encoder, Blink ve LCD başlatma
    motor_init();
    encoder_init();
    blink_init();

    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    lcd_init();

    // İlk zaman ayarı
    last_update_time = get_absolute_time();

    // 100 µs'de bir encoder güncellemesi için timer başlat
    repeating_timer_t encoder_timer;
    add_repeating_timer_us(-100, update_encoder_callback, NULL, &encoder_timer);

    // 50 ms'de bir veri yazdırmak için timer başlat
    repeating_timer_t print_timer;
    add_repeating_timer_ms(50, print_data_callback, NULL, &print_timer);

    // 1000 ms'de bir LED blink için timer başlat
    repeating_timer_t blink_timer;
    add_repeating_timer_ms(1000, blink_callback, NULL, &blink_timer);

    // Kalibrasyon işlemini başlat
    calibrate_encoder();

    // Motor kontrol döngüsü
    while (true) {
        int target_position = (rand() % 1001) - 500; // Rastgele hedef pozisyon (-500 ile +500 arasında)
        int steps = abs(target_position - get_position());
        bool direction = (target_position > get_position());

        // Motoru hareket ettir
        step_motor(steps, direction);
    }
}

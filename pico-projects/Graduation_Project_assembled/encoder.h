#ifndef ENCODER_H
#define ENCODER_H

#include "pico/stdlib.h"
#define TICKS_PER_REVOLUTION 1000 // Encoder'ın tam bir turdaki tıklama sayısı
// Encoder pin tanımları
#define PIN_A 2 // OUT A (GPIO 2)
#define PIN_B 3 // OUT B (GPIO 3)

// Fonksiyon Prototipleri
void encoder_init();
int encoder_get_ticks();
void encoder_reset();
float encoder_get_angle(); // Açı hesaplama fonksiyonu

#endif // ENCODER_H

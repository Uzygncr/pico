#ifndef BLINK_H
#define BLINK_H

#include "pico/stdlib.h"

// LED GPIO tanımı
#define LED_PIN CYW43_WL_GPIO_LED_PIN

// Blink için fonksiyon prototipleri
void blink_init();
bool blink_callback(repeating_timer_t *rt);

#endif // BLINK_H

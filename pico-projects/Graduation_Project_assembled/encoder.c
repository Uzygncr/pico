#include "encoder.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

// Global değişkenler
volatile int encoder_ticks = 0;
volatile int last_a = 0;

// Encoder ISR
void encoder_isr(uint gpio, uint32_t events) {
    int current_a = gpio_get(PIN_A);
    int current_b = gpio_get(PIN_B);

    if (current_a != last_a) {
        encoder_ticks += (current_a == current_b) ? 1 : -1; // Yön tayini

        // Encoder ticks'i sınırla
        if (encoder_ticks >= TICKS_PER_REVOLUTION) {
            encoder_ticks -= TICKS_PER_REVOLUTION; // 360'a ulaştı, sıfırdan başla
        } else if (encoder_ticks < 0) {
            encoder_ticks += TICKS_PER_REVOLUTION; // 0'ın altına düştü, 360'a dön
        }
    }

    last_a = current_a; // A sinyalini güncelle
}

// Encoder başlatma
void encoder_init() {
    gpio_init(PIN_A);
    gpio_init(PIN_B);

    gpio_set_dir(PIN_A, GPIO_IN);
    gpio_set_dir(PIN_B, GPIO_IN);

    gpio_set_irq_enabled_with_callback(PIN_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_isr);
    gpio_set_irq_enabled_with_callback(PIN_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_isr);
}

// Encoder ticks değerini döndürür
int encoder_get_ticks() {
    return encoder_ticks;
}

// Encoder'dan alınan açı değerini döndürür (derece cinsinden)
float encoder_get_angle() {
    // Tick değerini dereceye dönüştür
    float angle = ((float)encoder_ticks / TICKS_PER_REVOLUTION) * 360.0;

    // Açı değerini -180° ile +180° arasında sınırla
    if (angle >= 180.0) {
        angle -= 360.0; // Pozitif yönü negatif tarafa çevir
    } else if (angle < -180.0) {
        angle += 360.0; // Negatif yönü pozitif tarafa çevir
    }

    return angle;
}


// Encoder'i sıfırlar
void encoder_reset() {
    encoder_ticks = 0;
}

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#define PIN_A 2     // OUT A (GPIO 2)
#define PIN_B 3     // OUT B (GPIO 3)

volatile int encoder_ticks = 0;
volatile int last_a = 0;
volatile int last_b = 0;

void encoder_isr(uint gpio, uint32_t events) {
    int current_a = gpio_get(PIN_A);
    int current_b = gpio_get(PIN_B);

    // Yön tayini: A ve B sinyalleri arasındaki faz farkına göre
    if (current_a != last_a) {
        if (current_a == current_b) {
            encoder_ticks++; // Saat yönünde artış
        } else {
            encoder_ticks--; // Saat yönünün tersinde azalış
        }
    }

    last_a = current_a;
    last_b = current_b;
}

int main() {
    stdio_init_all();

    gpio_init(PIN_A);
    gpio_init(PIN_B);

    gpio_set_dir(PIN_A, GPIO_IN);
    gpio_set_dir(PIN_B, GPIO_IN);

    gpio_set_irq_enabled_with_callback(PIN_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_isr);
    gpio_set_irq_enabled_with_callback(PIN_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_isr);

    absolute_time_t last_print_time = get_absolute_time();

    while (true) {
        // 50 ms bekleme
        if (absolute_time_diff_us(last_print_time, get_absolute_time()) >= 50000) {
            printf("Encoder ticks: %d\n", encoder_ticks);
            last_print_time = get_absolute_time(); // Zamanı güncelle
        }
        tight_loop_contents(); // CPU'yu diğer işlevlere bırakmak için
    }
}

#include "blink.h"
#include "pico/cyw43_arch.h"

// LED başlatma fonksiyonu
void blink_init() {
    if (cyw43_arch_init()) {
        printf("Wi-Fi modülü başlatılamadı.\n");
    }
    cyw43_arch_gpio_put(LED_PIN, false); // LED başlangıç durumu
}

// LED blink için callback fonksiyonu
bool blink_callback(repeating_timer_t *rt) {
    static bool led_state = false;
    led_state = !led_state;
    cyw43_arch_gpio_put(LED_PIN, led_state);
    return true; // Timer devam etsin
}

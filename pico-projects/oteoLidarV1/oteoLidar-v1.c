#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

// Tanımlar
const uint ledPin=CYW43_WL_GPIO_LED_PIN;
const uint BLINK_INTERVAL_MS = 250;  // LED'in yanma süresi (ms)

int main() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        return -1;
    }
    //Ledi kapalı başlat
    cyw43_arch_gpio_put(ledPin, 0);
    
    // Zamanlayıcıyı başlat
    absolute_time_t next_blink_time = make_timeout_time_ms(BLINK_INTERVAL_MS);
    

    while (true) {
        // Zamanlayıcıyı kontrol et
        if (time_reached(next_blink_time)) {
            // LED'in durumunu tersine çevir
            bool ledOn = cyw43_arch_gpio_get(ledPin);
            cyw43_arch_gpio_put(ledPin, !ledOn);

            // Zamanlayıcıyı güncelle
            next_blink_time = delayed_by_ms(next_blink_time, BLINK_INTERVAL_MS);
        }

        // Diğer görevlerin çalışabilmesi için kısa bir süre uyuyun
        tight_loop_contents();  // İşlemciyi serbest bırakır
    }
}

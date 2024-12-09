#include "pico/stdlib.h"

#define STEP_PIN 15         // STEP pini
#define DIR_PIN 14          // DIR pini
#define MAX_SPEED 200       // Maksimum hız (us cinsinden minimum gecikme)
#define MIN_SPEED 1000      // Başlangıç ve bitiş hızı (us cinsinden maksimum gecikme)
#define ACCELERATION_STEP 2 // Her döngüde hız değişimi miktarı (us cinsinden)

void setup() {
    gpio_init(STEP_PIN);
    gpio_set_dir(STEP_PIN, GPIO_OUT);
    gpio_init(DIR_PIN);
    gpio_set_dir(DIR_PIN, GPIO_OUT);

    gpio_put(DIR_PIN, true);  // İlk yönü belirle (true veya false olarak değiştirilebilir)
}

void step_motor(int delay_us) {
    gpio_put(STEP_PIN, true);
    sleep_us(delay_us);
    gpio_put(STEP_PIN, false);
    sleep_us(delay_us);
}

void accelerate_motor() {
    int delay = MIN_SPEED;
    while (delay > MAX_SPEED) {
        step_motor(delay);
        delay -= ACCELERATION_STEP;  // Gecikmeyi kademeli olarak azaltarak hızlandır
    }
}

void decelerate_motor() {
    int delay = MAX_SPEED;
    while (delay < MIN_SPEED) {
        step_motor(delay);
        delay += ACCELERATION_STEP;  // Gecikmeyi kademeli olarak artırarak yavaşlat
    }
}

void loop() {
    while (true) {
        // Hızlanarak başla
        accelerate_motor();

        // Sabit hızda bir süre ilerle
        for (int i = 0; i < 2400; i++) {  // Belirli adım sayısı, ayarlanabilir
            step_motor(MAX_SPEED);
        }

        // Yavaşlayarak dur
        decelerate_motor();

        // Yön değiştir
        gpio_put(DIR_PIN, !gpio_get(DIR_PIN));
        sleep_ms(500);  // Yön değişiminden önce kısa bir bekleme süresi
    }
}

int main() {
    setup();
    loop();
    return 0;
}

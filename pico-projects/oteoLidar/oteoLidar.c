/*Basit düzeyde interrrupt içeren bir pico w blink kodu*/
#include    "pico/stdlib.h"
#include    "pico/cyw43_arch.h"

const uint ledPin=CYW43_WL_GPIO_LED_PIN;

int main(){
    stdio_init_all();
    if(cyw43_arch_init()) {
        return -1;
    }

    while(true) {
        cyw43_arch_gpio_put(ledPin, 1);
        sleep_ms(100);
        cyw43_arch_gpio_put(ledPin, 0);
        sleep_ms(100);

    }


}
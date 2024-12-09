#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"


////////////////
// Tanımlar ///////////////////////////////////////////////////////////////////////////////////////
//////////////

// Led Tanımları //
const uint ledPin=CYW43_WL_GPIO_LED_PIN; // Led pini cyw43_arch.h den alıyor (wifi modülü)
const uint BLINK_INTERVAL_MS = 250;  // LED'in yanma süresi (ms)
const uint PRINT_INTERVAL_MS = 250;  // Serialdan print etme süresi

// Encoder Tanımları //
const uint encoderA = 2; // Encoder A pini
const uint encoderB = 3; // Encoder B pini
volatile int position = 0;  // Encoder sayacı
volatile bool direction = 0;  // Yön bilgisi (0: saat yönünde, 1: saat yönünün tersinde)
volatile bool last_a = false;
volatile bool last_b = false;


// UART ayarları //
uart_inst_t *UART_ID = uart0;  // UART0 kullanıyoruz
const uint BAUD_RATE = 115200;  // Baud hızı


////////////////////
// Fonksiyonlar ///////////////////////////////////////////////////////////////////////////////////
//////////////////

void setup_uart() {
    uart_init(UART_ID, BAUD_RATE);  // UART'ı başlat ve baud hızını ayarla
    gpio_set_function(0, GPIO_FUNC_UART);  // UART TX pini
    gpio_set_function(1, GPIO_FUNC_UART);  // UART RX pini
}

// Yükselen kenar kesme işleyici
void encoder_a_isr(uint gpio, uint32_t events) {
    bool a = gpio_get(encoderA);  // A pin durumu
    bool b = gpio_get(encoderB);  // B pin durumu

    // A pininden gelen kesme
    if (gpio == encoderA) {
        if (a != last_a) {  // Yükselme veya düşüş kenarını kontrol et
            if (a == b) {
                position += 1;  // A pininde yükselme ve B pininde aynı değer
            } else {
                position -= 1;  // A pininde yükselme ve B pininde farklı değer
            }
        }
    }
    // B pininden gelen kesme
    else if (gpio == encoderB) {
        if (b != last_b) {  // Yükselme veya düşüş kenarını kontrol et
            if (a != b) {
                position += 1;  // B pininde yükselme ve A pininde farklı değer
            } else {
                position -= 1;  // B pininde yükselme ve A pininde aynı değer
            }
        }
    }

    last_a = a;
    last_b = b;
}

// Başlangıç fonksiyonu ///////////////////////////////////////////////////////////////////////////
void setup() {
    // GPIO pinlerini giriş olarak ayarla
     // GPIO pinlerini giriş olarak ayarla
    gpio_init(encoderA);
    gpio_set_dir(encoderA, GPIO_IN);
    gpio_pull_up(encoderA);  // Pull-up direnci etkinleştir
    gpio_init(encoderB);
    gpio_set_dir(encoderB, GPIO_IN);
    gpio_pull_up(encoderB);  // Pull-up direnci etkinleştir
    // Kesmeleri etkinleştir
    gpio_set_irq_enabled_with_callback(encoderA, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_a_isr);
    gpio_set_irq_enabled_with_callback(encoderB, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &encoder_a_isr);
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Ana kod başlangıcı
int main() {

    stdio_init_all();
    setup();
    setup_uart();  // UART'ı başlat

    if (cyw43_arch_init()) {
        return -1;
    }
    //Ledi kapalı başlat
    cyw43_arch_gpio_put(ledPin, 0);
    
    // Zamanlayıcıyı başlat
    absolute_time_t next_blink_time = make_timeout_time_ms(BLINK_INTERVAL_MS);
    absolute_time_t next_print_time = make_timeout_time_ms(PRINT_INTERVAL_MS);
    

    while (true) {
        // Zamanlayıcıyı kontrol et
        if (time_reached(next_blink_time)) {
            // LED'in durumunu tersine çevir
            bool ledOn = cyw43_arch_gpio_get(ledPin);
            cyw43_arch_gpio_put(ledPin, !ledOn);

            // Zamanlayıcıyı güncelle
            next_blink_time = delayed_by_ms(next_blink_time, BLINK_INTERVAL_MS);
        }

         if (time_reached(next_print_time)) {
            // Encoder sayısını yazdır
            printf("Encoder Count: %d\n", position);

            // Zamanlayıcıyı güncelle
            next_print_time = delayed_by_ms(next_print_time, PRINT_INTERVAL_MS);
        }
        
        

        // Diğer görevlerin çalışabilmesi için kısa bir süre uyuyun
        tight_loop_contents();  // İşlemciyi serbest bırakır
    }
}

#include "uart.h"
#include <stdio.h>

// UART başlatma fonksiyonu
void uart_init_config() {
    stdio_init_all(); // UART yapılandırmasını başlatır
}

// UART üzerinden encoder tick değerlerini yazdırma
void uart_send_encoder_ticks(int ticks) {
    printf("Encoder Ticks: %d\n", ticks);
}

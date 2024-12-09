#ifndef UART_H
#define UART_H

#include "pico/stdlib.h"

// UART başlatma prototipi
void uart_init_config();
void uart_send_encoder_ticks(int ticks);

#endif // UART_H

#ifndef LCD_H
#define LCD_H

#include "hardware/i2c.h"

// I2C Pin Tanımları
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1
#define LCD_ADDR 0x27

// Fonksiyon Prototipleri
void lcd_init();
void lcd_clear();
void lcd_set_cursor(int line);
void lcd_print(const char *str);

#endif // LCD_H

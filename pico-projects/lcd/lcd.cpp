#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C pinleri
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1
#define LCD_ADDR 0x27

// LCD kontrol bayrakları
#define LCD_CMD 0
#define LCD_CHR 1
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0b00000100

void lcd_toggle_enable(uint8_t bits);
void lcd_send_byte(uint8_t bits, uint8_t mode);
void lcd_init();
void lcd_clear();
void lcd_set_cursor(int line);
void lcd_print(const char *str);

void lcd_toggle_enable(uint8_t bits) {
    sleep_us(500);
    bits |= LCD_ENABLE;
    i2c_write_blocking(i2c0, LCD_ADDR, &bits, 1, false);
    sleep_us(500);
    bits &= ~LCD_ENABLE;
    i2c_write_blocking(i2c0, LCD_ADDR, &bits, 1, false);
    sleep_us(500);
}

void lcd_send_byte(uint8_t bits, uint8_t mode) {
    uint8_t high_nibble = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    uint8_t low_nibble = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_blocking(i2c0, LCD_ADDR, &high_nibble, 1, false);
    lcd_toggle_enable(high_nibble);

    i2c_write_blocking(i2c0, LCD_ADDR, &low_nibble, 1, false);
    lcd_toggle_enable(low_nibble);
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_CMD);
    lcd_send_byte(0x03, LCD_CMD);
    lcd_send_byte(0x03, LCD_CMD);
    lcd_send_byte(0x02, LCD_CMD);

    lcd_send_byte(0x28, LCD_CMD); // 4-bit mode, 2-line
    lcd_send_byte(0x0C, LCD_CMD); // Display on, no cursor
    lcd_send_byte(0x06, LCD_CMD); // Entry mode set
    lcd_send_byte(0x01, LCD_CMD); // Clear display
    sleep_ms(2);
}

void lcd_clear() {
    lcd_send_byte(0x01, LCD_CMD); // Clear display
    sleep_ms(2);
}

void lcd_set_cursor(int line) {
    int addr = (line == 0) ? 0x80 : 0xC0; // Satır 0 veya 1 için başlangıç adresi
    lcd_send_byte(addr, LCD_CMD);
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_byte(*str++, LCD_CHR);
    }
}

int main() {
    stdio_init_all();

    // I2C başlatma
    i2c_init(i2c0, 100 * 1000); // 100kHz
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // LCD başlatma
    lcd_init();

    // Mesaj gösterme
    lcd_clear();
    lcd_set_cursor(0); // İlk satır
    lcd_print("Merhaba!");
    lcd_set_cursor(1); // İkinci satır
    lcd_print("Dunya!");

    while (true) {
        // Sonsuz döngü
    }

    return 0;
}

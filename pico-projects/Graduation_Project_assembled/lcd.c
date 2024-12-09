#include "lcd.h"
#include "pico/stdlib.h"

// LCD Enable Toggling
void lcd_toggle_enable(uint8_t bits) {
    sleep_us(500);
    bits |= 0x04;
    i2c_write_blocking(i2c0, LCD_ADDR, &bits, 1, false);
    sleep_us(500);
    bits &= ~0x04;
    i2c_write_blocking(i2c0, LCD_ADDR, &bits, 1, false);
    sleep_us(500);
}

// LCD Byte Gönderme
void lcd_send_byte(uint8_t bits, uint8_t mode) {
    uint8_t high_nibble = mode | (bits & 0xF0) | 0x08;
    uint8_t low_nibble = mode | ((bits << 4) & 0xF0) | 0x08;

    i2c_write_blocking(i2c0, LCD_ADDR, &high_nibble, 1, false);
    lcd_toggle_enable(high_nibble);

    i2c_write_blocking(i2c0, LCD_ADDR, &low_nibble, 1, false);
    lcd_toggle_enable(low_nibble);
}

// LCD Başlatma
void lcd_init() {
    lcd_send_byte(0x03, 0);
    lcd_send_byte(0x03, 0);
    lcd_send_byte(0x03, 0);
    lcd_send_byte(0x02, 0);

    lcd_send_byte(0x28, 0); // 4-bit mode, 2-line
    lcd_send_byte(0x0C, 0); // Display on, no cursor
    lcd_send_byte(0x06, 0); // Entry mode set
    lcd_send_byte(0x01, 0); // Clear display
    sleep_ms(2);
}

// LCD Temizleme
void lcd_clear() {
    lcd_send_byte(0x01, 0); // Clear display
    sleep_ms(2);
}

// LCD İmleç Ayarlama
void lcd_set_cursor(int line) {
    int addr = (line == 0) ? 0x80 : 0xC0; // Satır 0 veya 1 için başlangıç adresi
    lcd_send_byte(addr, 0);
}

// LCD'ye Metin Yazdırma
void lcd_print(const char *str) {
    while (*str) {
        lcd_send_byte(*str++, 1);
    }
}

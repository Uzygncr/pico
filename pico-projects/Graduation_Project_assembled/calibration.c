#include "calibration.h"
#include "pico/stdlib.h"

// Kalibrasyon fonksiyonu
void calibrate_encoder() {
    lcd_clear();
    lcd_set_cursor(0);
    lcd_print("Makineye Ellemeyin");

    int last_ticks = encoder_get_ticks();
    int stable_time = 0; // Değişmeyen süre (ms cinsinden)
    const int calibration_check_interval = 1000; // 1000 ms aralıklarla kontrol
    const int stable_threshold = 5000; // 5 saniye (5000 ms)
    int dot_state = 0; // Animasyon durumu

    while (stable_time < stable_threshold) {
        sleep_ms(calibration_check_interval);

        // Encoder tick kontrolü
        int current_ticks = encoder_get_ticks();
        if (current_ticks == last_ticks) {
            stable_time += calibration_check_interval;
        } else {
            stable_time = 0;
        }
        last_ticks = current_ticks;

        // Animasyon güncelleme
        dot_state = (dot_state + 1) % 6; // 0, 1, 2, 3, 4 , 5 arasında döngü yapar
        lcd_clear();
        lcd_set_cursor(0);
        lcd_print("Makineye Ellemeyin");
        lcd_set_cursor(1);
        lcd_print("Kalibrasyon");

        // Nokta ekleme
        for (int i = 0; i < dot_state; i++) {
            lcd_print(".");
        }
    }

    // Kalibrasyon tamamlandı
    encoder_reset();
    lcd_clear();
    lcd_set_cursor(0);
    lcd_print("Kalibrasyon");
    lcd_set_cursor(1);
    lcd_print("Tamamlandi!");
    sleep_ms(5000); // 5 saniye bekle
}

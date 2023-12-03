#include "APA102.h"

#include "spi.h"

static uint8_t num_leds;

static void send_start_frame(void) {
    spi_write_read(0);
    spi_write_read(0);
    spi_write_read(0);
    spi_write_read(0);
}

static void send_end_frame(void) {
    spi_write_read(1);
    spi_write_read(1);
    spi_write_read(1);
    spi_write_read(1);
}

static void send_color_frame(uint8_t power, uint8_t r, uint8_t g, uint8_t b) {
    spi_write_read(0b11100000 | power);
    spi_write_read(b);
    spi_write_read(g);
    spi_write_read(r);
}

void APA102_init(uint8_t config_num_leds) {
    num_leds = config_num_leds;
}

void APA102_set_color(uint8_t power, uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t i = 0; i < num_leds; ++i) {
        send_color_frame(power, r, g, b);
    }
}
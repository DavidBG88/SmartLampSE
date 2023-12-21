#include "iAQCore.h"

#include "i2c.h"

#define IAQCORE_I2C_ADDRESS 0b1011010

void IAQCORE_init(void) {
    I2C_init();
}

IAQCORE_Reading IAQCORE_read(void) {
    I2C_start();
    I2C_write_address(IAQCORE_I2C_ADDRESS, I2C_MODE_READ_BIT);

    IAQCORE_Reading reading;

    I2C_read_n_bytes_reverse((uint8_t*)&reading.eco2_ppm, sizeof(reading.eco2_ppm));
    I2C_read_n_bytes_reverse((uint8_t*)&reading.status, sizeof(reading.status));
    I2C_read_n_bytes_reverse((uint8_t*)&reading.resistance, sizeof(reading.resistance));
    I2C_read_n_bytes_reverse((uint8_t*)&reading.etvoc_ppb, sizeof(reading.etvoc_ppb));

    I2C_stop();

    return reading;
}
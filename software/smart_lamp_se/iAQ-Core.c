#include "iAQ-Core.h"

#include "i2c.h"

#define IAQCORE_I2C_ADDRESS 0b1011010

iAQCore_Reading iAQCore_read() {
    i2c_start();
    i2c_write_address(IAQCORE_I2C_ADDRESS, I2C_MODE_READ_BIT);

    iAQCore_Reading reading;

    i2c_read_n_bytes(reading.eco2_ppm, sizeof(reading.eco2_ppm));
    i2c_read_n_bytes(reading.status, sizeof(reading.status));
    i2c_read_n_bytes(reading.resistance, sizeof(reading.resistance));
    i2c_read_n_bytes(reading.etvoc_ppb, sizeof(reading.etvoc_ppb));

    i2c_stop();
}
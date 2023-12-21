#include "VEML7700.h"

#include "i2c.h"

/*
float getGainValue(void) {
  switch (getGain()) {
  case VEML7700_GAIN_1_8:
    return 0.125;
  case VEML7700_GAIN_1_4:
    return 0.25;
  case VEML7700_GAIN_1:
    return 1;
  case VEML7700_GAIN_2:
    return 2;
  default:
    return -1;
  }
}

float getResolution(void) {
  return MAX_RES * (IT_MAX / getIntegrationTimeValue()) *
         (GAIN_MAX / getGainValue());
}

float computeLux(uint16_t rawALS, bool corrected) {
    float lux = getResolution() * rawALS;
    if (corrected)
        lux = (((6.0135e-13 * lux - 9.3924e-9) * lux + 8.1488e-5) * lux + 1.0023) * lux;
    return lux;
}
*/

uint16_t VEML7700_read_light_lux() {
    I2C_start();
    I2C_write_address(VEML7700_I2C_ADDRESS, I2C_MODE_WRITE_BIT);
    I2C_write(VEML7700_ALS_REG);

    I2C_restart();
    I2C_write_address(VEML7700_I2C_ADDRESS, I2C_MODE_READ_BIT);

    uint16_t data;
    data = I2C_read(I2C_ACK_OK_BIT) | (((uint16_t)I2C_read(I2C_ACK_OK_BIT)) << 8);

    I2C_stop();

    return data;
}

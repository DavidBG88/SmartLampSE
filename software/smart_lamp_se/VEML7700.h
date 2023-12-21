#ifndef VEML7700_H
#define VEML7700_H

#ifdef __cplusplus
extern "C" {
#endif

#define VEML7700_I2C_ADDRESS 0x10
#define VEML7700_ALS_REG 0x04

#include <stdint.h>

uint16_t VEML7700_read_light_lux();

#ifdef __cplusplus
}
#endif

#endif  // VEML7700_H

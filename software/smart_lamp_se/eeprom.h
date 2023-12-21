#ifndef EEPROM_H
#define EEPROM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Read the the byte at `address` from the data EEPROM.
 * @brief Reads the byte at `address` from the data EEPROM.
 */
uint8_t EEPROM_read(uint8_t address);

/**
 * Write data to the data EEPROM at the specified address.
 * @brief Writes `data` to the `address` address in the data EEPROM.
 * @return True if the write was successfull, false otherwise.
 */
bool EEPROM_write(uint8_t address, uint8_t data);

#ifdef __cplusplus
}
#endif

#endif  // EEPROM_H

/* 
 * File:   i2c.h
 * Author: norberto
 *
 * Created on 29 de junio de 2022, 11:43
 */

#ifndef I2C_H
#define I2C_H

#define I2C_ACK_OK_BIT 0
#define I2C_ACK_ERR_BIT 1

#define I2C_MODE_READ_BIT 1
#define I2C_MODE_WRITE_BIT 0

#include <stdint.h>

void I2C_init(void);

void I2C_start(void);

void I2C_stop(void);

void I2C_restart(void);

uint8_t I2C_write(uint8_t byte);

uint8_t I2C_write_address(uint8_t address, uint8_t read_write_bit);

// master_ack = 1 => acknowledge. master_ack = 0 => not acknowledge
// Master must start (or restart) comunications first and send
// address with the lower bit set.
uint8_t I2C_read(uint8_t master_ack);

void I2C_read_n_bytes(uint8_t* bytes, uint8_t num_bytes);

void I2C_read_n_bytes_reverse(uint8_t* bytes, uint8_t num_bytes);

#endif /* I2C_H */

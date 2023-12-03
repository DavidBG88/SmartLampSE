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

void i2c_init(void);

void i2c_start(void);

void i2c_stop(void);

void i2c_rstart(void);

uint8_t i2c_write(uint8_t I2C_data);

uint8_t i2c_write_address(uint8_t address, uint8_t read_write_bit);

// master_ack = 1 => acknowledge. master_ack = 0 => not acknowledge
// Master must start (or restart) comunications first and send
// address with the lower bit set.
uint8_t i2c_read(uint8_t master_ack);

void i2c_read_n_bytes(uint8_t* bytes, uint8_t num_bytes);

#endif /* I2C_H */

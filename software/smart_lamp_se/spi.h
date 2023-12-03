/* 
 * File:   spi-master-v1.h
 * Author: norberto
 *
 * Created on 12 de julio de 2022, 11:15
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>

#define spi_clk PORTBbits.RB4
#define spi_dat_out PORTBbits.RB5
#define spi_dat_in PORTBbits.RB6

uint8_t spi_write_read(uint8_t one_byte);

#endif  // SPI_H

// General routines for i2c comunications
#include "i2c.h"

#define i2c_ack 1
#define i2c_nack 0

#include <xc.h>

void wait_MSSP(void) {
    while (!PIR1bits.SSPIF)
        ;

    PIR1bits.SSPIF = 0;
    return;
}

void i2c_start(void) {
    SSPCON2bits.SEN = 1;
    wait_MSSP();  //Wait to complete
    return;
}

void i2c_stop(void) {
    SSPCON2bits.PEN = 1;
    wait_MSSP();  //Wait to complete
    return;
}

void i2c_rstart(void) {
    SSPCON2bits.RSEN = 1;
    wait_MSSP();  //Wait to complete
    return;
}

uint8_t i2c_write(uint8_t I2C_data) {
    SSPBUF = I2C_data;
    wait_MSSP();                             //Wait to complete
    return ((uint8_t)!SSPCON2bits.ACKSTAT);  //Returns 1 if byte is acknowledge
}

// master_ack = 1 => acknowledge. master_ack = 0 => not acknowledge
// Master must start (or restart) comunications first and send
// address with the lower bit set.
uint8_t i2c_read(uint8_t master_ack) {
    uint8_t I2C_data;

    SSPCON2bits.RCEN = 1;
    wait_MSSP();  // Wait for data

    I2C_data = SSPBUF;  // It is necessary to read the buffer for SCL progress

    if (master_ack)
        SSPCON2bits.ACKDT = 0;
    else
        SSPCON2bits.ACKDT = 1;

    SSPCON2bits.ACKEN = 1;
    wait_MSSP();
    return (I2C_data);
}

uint8_t i2c_write_address(uint8_t address, uint8_t read_write_bit) {
    return i2c_write((uint8_t)(address << 1) | (read_write_bit & 0b1));
}

void i2c_read_n_bytes(uint8_t* bytes, uint8_t num_bytes) {
    for (uint8_t i = 0; i < num_bytes; ++i) {
        bytes[i] = i2c_read(I2C_ACK_OK_BIT);
    }
}
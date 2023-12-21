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

void I2C_init(void) {
    SSPCONbits.SSPM = 0b1000;  // Set MSSP to I2C master mode

    // Clock = FOSC / (4 * (SSPADD + 1))
    // 100 kHz = 20 Mhz / (4 * (SSPADD + 1))
    // 4 * (SSPADD + 1) = (20 * 10^6) / (10^5)
    // SSPADD + 1 = 200 / 4
    // SSPADD = 50 - 1
    // SSPADD = 49
    SSPADD = 49;  // 100 kHz when FOSC = 20 Mhz

    SSPSTATbits.SMP = 1;  // Disable slew rate control

    SSPCONbits.SSPEN = 1;  // Enable MSSP
}

void I2C_start(void) {
    SSPCON2bits.SEN = 1;
    wait_MSSP();  //Wait to complete
    return;
}

void I2C_stop(void) {
    SSPCON2bits.PEN = 1;
    wait_MSSP();  //Wait to complete
    return;
}

void I2C_restart(void) {
    SSPCON2bits.RSEN = 1;
    wait_MSSP();  //Wait to complete
    return;
}

uint8_t I2C_write(uint8_t byte) {
    SSPBUF = byte;
    wait_MSSP();                             //Wait to complete
    return ((uint8_t)!SSPCON2bits.ACKSTAT);  //Returns 1 if byte is acknowledge
}

// master_ack = 1 => acknowledge. master_ack = 0 => not acknowledge
// Master must start (or restart) comunications first and send
// address with the lower bit set.
uint8_t I2C_read(uint8_t master_ack) {
    uint8_t byte;

    SSPCON2bits.RCEN = 1;
    wait_MSSP();  // Wait for data

    byte = SSPBUF;  // It is necessary to read the buffer for SCL progress

    if (master_ack)
        SSPCON2bits.ACKDT = 0;
    else
        SSPCON2bits.ACKDT = 1;

    SSPCON2bits.ACKEN = 1;
    wait_MSSP();
    return (byte);
}

uint8_t I2C_write_address(uint8_t address, uint8_t read_write_bit) {
    return I2C_write((uint8_t)(address << 1) | (read_write_bit & 0b1));
}

void I2C_read_n_bytes(uint8_t* bytes, uint8_t num_bytes) {
    for (uint8_t i = 0; i < num_bytes; ++i) {
        bytes[i] = I2C_read(I2C_ACK_OK_BIT);
    }
}

void I2C_read_n_bytes_reverse(uint8_t* bytes, uint8_t num_bytes) {
    for (uint8_t i = num_bytes; i > 0; --i) {
        bytes[i - 1] = I2C_read(I2C_ACK_OK_BIT);
    }
}
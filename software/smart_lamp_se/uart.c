#include "uart.h"

#include <xc.h>

static inline void wait_until_write_finished() {
    //while (!PIR1bits.TXIF) {}  // Simulator
    while (!TXSTAbits.TRMT) {}  // Hardware
}

void uart_init() {
    // Set 8 bit asynchronous
    TXSTAbits.BRGH = 0;
    BAUDCTLbits.BRG16 = 0;
    // Baud Rate = FOSC / (64 (spbrg + 1))

    TXSTAbits.SYNC = 0;  // Asynchronous
    RCSTAbits.SPEN = 1;  // Serial port enable

    TXSTAbits.TX9 = 0;  // TX 8 data bit
    RCSTAbits.RX9 = 0;  // RX 8 data bit

    RCSTAbits.OERR = 0;  // Can't receive if overrun error flag is set

    TXSTAbits.TXEN = 1;  // Enable transmission
    RCSTAbits.CREN = 1;  // Enable reception
}

void uart_set_baud_rate(uint16_t spbrg) {
    SPBRG = (uint8_t)spbrg;
    SPBRGH = (uint8_t)(spbrg >> 8);
}

void uart_write_byte(uint8_t byte) {
    wait_until_write_finished();
    TXREG = byte;
}

void uart_write_data(const uint8_t* data, size_t length) {
    const uint8_t* last_byte_ptr = data + length;
    for (const uint8_t* byte_ptr = data; byte_ptr < last_byte_ptr; ++byte_ptr)
        uart_write_byte(*byte_ptr);
}

void uart_puts(const char* str) {
    for (const char* char_ptr = str; *char_ptr != '\0'; ++char_ptr)
        uart_write_byte(*char_ptr);
}

bool uart_read_available() {
    return PIR1bits.RCIF;
}

uint8_t uart_read_byte() {
    while (!uart_read_available()) {}
    return RCREG;
}

void uart_read_n_bytes(uint8_t byte_num, uint8_t* bytes) {
    for (uint8_t i = 0; i < byte_num; ++i)
        bytes[i] = uart_read_byte();
}
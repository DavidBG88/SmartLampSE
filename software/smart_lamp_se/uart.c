#include "uart.h"

#include <xc.h>

static inline void wait_until_write_finished() {
    //while (!PIR1bits.TXIF) {}  // Simulator
    while (!TXSTAbits.TRMT) {}  // Hardware
}

static inline void enable_transmission() {
    TXSTAbits.TXEN = 1;
}

static inline void disable_transmision() {
    wait_until_write_finished();
    TXSTAbits.TXEN = 0;
}

// To call raw_write_byte, tansmission must be enabled
static inline void raw_write_byte(uint8_t byte) {
    wait_until_write_finished();
    TXREG = byte;
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

    uart_enable_reception();
}

void uart_set_baud_rate(uint16_t spbrg) {
    SPBRG = (uint8_t)spbrg;
    SPBRGH = (uint8_t)(spbrg >> 8);
}

void uart_write_byte(uint8_t byte) {
    enable_transmission();

    raw_write_byte(byte);

    disable_transmision();
}

void uart_write_data(const uint8_t* data, size_t length) {
    enable_transmission();

    const uint8_t* last_byte_ptr = data + length;
    for (const uint8_t* byte_ptr = data; byte_ptr < last_byte_ptr; ++byte_ptr)
        raw_write_byte(*byte_ptr);

    disable_transmision();
}

void uart_puts(const char* str) {
    enable_transmission();

    for (const char* char_ptr = str; *char_ptr != '\0'; ++char_ptr)
        raw_write_byte(*char_ptr);

    disable_transmision();
}

void uart_enable_reception() {
    RCSTAbits.CREN = 1;
}

void uart_disable_reception() {
    RCSTAbits.CREN = 0;
}

bool uart_read_available() {
    return PIR1bits.RCIF;
}

uint8_t uart_read_byte() {
    while (!PIR1bits.RCIF) {}
    return RCREG;
}

void uart_read_n_bytes(uint8_t byte_num, uint8_t* bytes) {
    for (uint8_t i = 0; i < byte_num; ++i)
        bytes[i] = uart_read_byte();
}
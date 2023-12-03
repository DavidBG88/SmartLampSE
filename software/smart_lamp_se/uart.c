#include "uart.h"

#include <xc.h>

inline void wait_until_write_finished(void) {
    //while (!PIR1bits.TXIF) {}  // Simulator
    while (!TXSTAbits.TRMT) {}  // Hardware
}

void UART_init(void) {
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

void UART_set_baud_rate(uint16_t spbrg) {
    SPBRG = (uint8_t)spbrg;
    SPBRGH = (uint8_t)(spbrg >> 8);
}

void UART_write_byte(uint8_t byte) {
    wait_until_write_finished();
    TXREG = byte;
}

void UART_write_n_bytes(const uint8_t* bytes, uint8_t num_bytes) {
    const uint8_t* last_byte_ptr = bytes + num_bytes;
    for (const uint8_t* byte_ptr = bytes; byte_ptr < last_byte_ptr; ++byte_ptr)
        UART_write_byte(*byte_ptr);
}

void UART_puts(const char* str) {
    for (const char* char_ptr = str; *char_ptr != '\0'; ++char_ptr)
        UART_write_byte(*char_ptr);
}

bool UART_read_available(void) {
    return PIR1bits.RCIF;
}

uint8_t UART_read_byte(void) {
    while (!UART_read_available()) {}
    return RCREG;
}

void UART_read_n_bytes(uint8_t* bytes, uint8_t byte_num) {
    for (uint8_t i = 0; i < byte_num; ++i)
        bytes[i] = UART_read_byte();
}
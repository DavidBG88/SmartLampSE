#include "uart.h"

#include <xc.h>

void init_uart() {
    // Set 8 bit asynchronous
    TXSTAbits.BRGH = 0;
    BAUDCTLbits.BRG16 = 0;
    // Baud Rate = FOSC / (64 (spbrg + 1))

    TXSTAbits.SYNC = 0;  // Asynchronous
    TXSTAbits.TX9 = 0;   // TX 8 data bit
    RCSTAbits.RX9 = 0;   // TX 8 data bit

    PIE1bits.TXIE = 0;  // Disable TX interrupt
    PIE1bits.RCIE = 0;  // Disable RX interrupt

    RCSTAbits.SPEN = 1;  // Serial port enable

    TXSTAbits.TXEN = 0;  // Reset transmitter
    TXSTAbits.TXEN = 1;  // Enable transmitter
}

void set_uart_baud_rate(uint16_t spbrg) {
    SPBRG = (uint8_t)spbrg;
    SPBRGH = (uint8_t)(spbrg >> 8);
}

void send_uart_byte(uint8_t byte) {
    while (!PIR1bits.TXIF) {}  // Simulator
    // while (!TXSTAbits.TRMT) {} // Hardware
    TXREG = byte;
}

bool uart_input_data_ready() {
    return false;
}

uint8_t read_uart_byte() {
    return 0;
}

void putch(char txData) {
    send_uart_byte((char)txData);
}
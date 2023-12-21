#include "uart_rx_test.h"

#include "uart.h"

void uart_rx_test() {
    UART_init();
    UART_set_baud_rate(32);  // 9600 bauds / sec

    while (1) {
        UART_write_byte(UART_read_byte());
    }
}
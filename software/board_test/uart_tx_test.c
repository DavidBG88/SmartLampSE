#include "uart_tx_test.h"

#include <stdint.h>
#include <xc.h>

#define _XTAL_FREQ 20000000  // necessary for __delay_us

const uint8_t tx_data[] = {'A', 'B', 'C', 0b255, 0b0};
const uint8_t tx_data_len = sizeof(tx_data) / sizeof(tx_data[0]);

void uart_tx_test() {
	UART_init();
	UART_set_baud_rate(32); // 9600 bauds / sec

	while (1) {
		UART_write_n_bytes(tx_data, tx_data_len);
		__delay_ms(1000);
	}
}
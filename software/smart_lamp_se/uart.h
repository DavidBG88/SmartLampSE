#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UART init function.
 * @brief Initializes the UART module.
 * @warning Must be called before any other UART function.
 */
void init_uart();

/**
 * Send byte over UART.
 * @brief Sends a byte over the UART interface.
 */
void send_uart_byte(uint8_t byte);

/**
 * Check if the input UART queue is not empty.
 * @brief Returns true if the UART queue is not empty.
 * @return True if the UART queue is not empty.
 */
bool uart_input_data_ready();

/**
 * Read a byte from the UART input queue.
 * @brief Reads a byte from the UART input queue.
 * @return The first byte from the UART input queue.
 */
uint8_t read_uart_byte();

#ifdef __cplusplus
}
#endif

#endif  // UART_H

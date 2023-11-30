#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * UART init function.
 * @brief Initializes the UART module.
 * @warning Must be called before any other UART function.
 */
void uart_init();

/**
 * UART set baud rate.
 * @brief Sets the UART module's baud rate to according to the following equation [baud_rate = FOSC / (64 * (spbrg + 1))].
 */
void uart_set_baud_rate(uint16_t spbrg);

/**
 * Send byte over UART.
 * @brief Sends a byte over the UART interface.
 */
void uart_write_byte(uint8_t byte);

/**
 * Send an array of bytes over UART.
 * @brief Sends an array of bytes over the UART interface.
 */
void uart_write_data(const uint8_t* data, size_t length);

/**
 * Print a string over UART
 * @brief Prints a string over the UART interface.
 */
void uart_puts(const char* str);

/**
 * Enable UART reception.
 * @brief Enables UART reception. It is enabled by default, but can be disabled with `uart_disable_recpetion`.
 */
void uart_enable_reception();

/**
 * Disable UART reception.
 * @brief Disables UART reception.
 */
void uart_disable_reception();

/**
 * Check if the input UART queue is not empty.
 * @brief Returns true if the UART queue is not empty.
 * @return True if the UART queue is not empty.
 */
bool uart_read_available();

/**
 * Read a byte from the UART input queue.
 * @brief Reads a byte from the UART input queue.
 * @return The first byte from the UART input queue.
 */
uint8_t uart_read_byte();

#ifdef __cplusplus
}
#endif

#endif  // UART_H

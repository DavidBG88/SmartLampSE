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
void UART_init(void);

/**
 * UART set baud rate.
 * @brief Sets the UART module's baud rate to according to the following equation [baud_rate = FOSC / (64 * (spbrg + 1))].
 */
void UART_set_baud_rate(uint16_t spbrg);

/**
 * Send byte over UART.
 * @brief Sends a byte over the UART interface.
 */
void UART_write_byte(uint8_t byte);

/**
 * Send an array of bytes over UART.
 * @brief Sends an array of bytes over the UART interface.
 */
void UART_write_n_bytes(const uint8_t* bytes, uint8_t byte_num);

/**
 * Print a string over UART
 * @brief Prints a string over the UART interface.
 */
void UART_puts(const char* str);

/**
 * Check if the input UART queue is not empty.
 * @brief Returns true if the UART queue is not empty.
 * @return True if the UART queue is not empty.
 */
bool UART_read_available(void);

/**
 * Read a byte from the UART input queue.
 * @brief Reads a byte from the UART input queue.
 * @return The first byte from the UART input queue.
 */
uint8_t UART_read_byte(void);

/**
 * Read the next n bytes from the UART input queue.
 * @brief Reads the next n bytes from the UART interface and stores them in `bytes`.
 */
void UART_read_n_bytes(uint8_t* bytes, uint8_t byte_num);

#ifdef __cplusplus
}
#endif

#endif  // UART_H

#ifndef TIMING_H
#define TIMING_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Timing init function
 * @brief Initializes the Timing module.
 * @warning Must be called before any other Timing function.
 * @warning The Timing module takes ownership of the Timer0.
 * @warning General Interrupt Enable flag must be enabled (INTCONbits.GIE = 1).
 */
void init_timing();

/**
 * Timing timer0 interrupt handler.
 * @brief Timer0 interrupt needed for the Timing module to work.
 * @warning Must be added to the interrupt handler and called when the Timer0 interrupt flag is raised.
 * @warning General Interrupt Enable flag must be enabled.
 */
void timing_tmr0_interrupt_handler();

/**
 * Set the Timing module's tick duration.
 * @brief Sets the Timing module's tick duration.
 */
void set_tick_duration(uint8_t tmr0, uint8_t ps);

/**
 * Add a function to the Timing's callback queue.
 * @brief Sets a timer for the given function so it is called every n ticks.
 * @warning If two functions are called at the same time, they will be called in the order specified by their priorities.
 */
void run_every_n_ticks(uint8_t ticks, uint8_t priority, void (*function)(void));

#ifdef __cplusplus
}
#endif

#endif  // TIMING_H

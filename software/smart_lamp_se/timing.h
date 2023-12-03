#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TMR0Prescaler {
    PR1to2 = 0,
    PR1to4,
    PR1to8,
    PR1to16,
    PR1to32,
    PR1to64,
    PR1to128,
    PR1to256,
    PR1to1
} TMR0Prescaler;

/**
 * Timing init function
 * @brief Initializes the Timing module.
 * @warning Must be called before any other Timing function.
 * @warning The Timing module takes ownership of the Timer0.
 * @warning General Interrupt Enable flag must be enabled (INTCONbits.GIE = 1).
 * @warning Timer0 INterrupt Enable flag must be enabled (INTCONbits.T0IE = 1).
 */
void init_timing(void);

/**
 * Timing timer0 interrupt handler.
 * @brief Timer0 interrupt needed for the Timing module to work.
 * @warning Must be added to the interrupt handler and called when the Timer0 interrupt flag is raised.
 * @warning General Interrupt Enable flag must be enabled.
 */
void timing_tmr0_interrupt_handler(void);

/**
 * Set the Timing module's tick duration.
 * @brief Sets the Timing module's tick duration.
 */
void set_tick_duration(uint8_t tmr0, TMR0Prescaler pr);

/**
 * Add a function to the Timing's callback queue.
 * @brief Sets a timer for the given function so it is called every n ticks.
 * @warning In case two functions should run simultaneously, priority will be given in the order they were added to the Timing module's function list.
 * @warning If two functions are called at the same time, they will be called in the order specified by their priorities.
 */
void run_every_n_ticks(uint16_t ticks, void (*function_ptr)(void));

#ifdef __cplusplus
}
#endif

#endif  // TIMING_H

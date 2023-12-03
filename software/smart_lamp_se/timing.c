#include "timing.h"

#include <stddef.h>
#include <xc.h>

#define MAX_TIMING_FUNCTIONS 8

typedef struct TimingFunction {
    void (*function_ptr)(void);
    uint16_t ticks_left;
    uint16_t ticks;
} TimingFunction;

static uint8_t next_func_idx = 0;
static TimingFunction timing_functions[MAX_TIMING_FUNCTIONS];

static uint8_t tmr0_conf = 0;
static uint8_t pr_bits_conf = 0;

void init_timing(void) {
    OPTION_REGbits.T0CS = 0;
    INTCONbits.T0IF = 0;
}

void timing_tmr0_interrupt_handler(void) {
    TMR0 = tmr0_conf;

    for (uint8_t i = 0; i < next_func_idx; ++i) {
        TimingFunction* tf = &timing_functions[i];

        tf->ticks_left -= 1;

        if (tf->ticks_left == 0) {
            (*tf->function_ptr)();
            tf->ticks_left = tf->ticks;
        }
    }
}

void set_tick_duration(uint8_t tmr0, TMR0Prescaler pr) {
    // Store configuration
    tmr0_conf = tmr0;
    pr_bits_conf = pr;

    if (pr == PR1to1) {  // For 1:1 prescaler, wdt should own the prescaler
        OPTION_REGbits.PSA = 1;
    } else {  // For 1:x such that x>1 prescaler, tmr0 should own the prescaler
        OPTION_REGbits.PSA = 0;
        OPTION_REGbits.PS = pr;
    }

    TMR0 = tmr0_conf;  // Start the timer
}

void run_every_n_ticks(uint16_t ticks, void (*function_ptr)(void)) {
    TimingFunction* tf = &timing_functions[next_func_idx];
    ++next_func_idx;

    tf->function_ptr = function_ptr;
    tf->ticks_left = tf->ticks = ticks;
}

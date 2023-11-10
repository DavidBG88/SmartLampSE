#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * PMW init function.
     * @brief Initializes the pwm module.
     * @warning Must be called before any other pwm function.
     */
    void init_pwm();

    /**
     * PWM timer2 interrupt handler.
     * @brief Timer2 interrupt needed for the PWM module to work.
     * @warning Must be added to the interrupt handler and called when the Timer2 interrupt flag is raised.
     */
    void pwm_tmr2_interrupt_handler();

    /**
     * PWM duty cycle configuration function.
     * @brief Sets the PWM module's duty cycle.
     */
    void set_pwm_duty_cycle(uint16_t duty_cycle);

    /**
     * PWM duty cycle percentage configuration function.
     * @brief Sets the percentage of time the PWM signal is high.
     */
    void set_pwm_duty_cycle_percentage(uint8_t percentage);

#ifdef __cplusplus
}
#endif

#endif // PWM_H

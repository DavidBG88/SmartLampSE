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
     * @warning General Interrupt Enable flag must be enabled.
     */
    void init_pwm();

    /**
     * PWM timer2 interrupt handler.
     * @brief Timer2 interrupt needed for the PWM module to work.
     * @warning Must be added to the interrupt handler and called when the Timer2 interrupt flag is raised.
     * @warning General Interrupt Enable flag must be enabled.
     */
    void pwm_tmr2_interrupt_handler();

    /**
     * PWM duty cycle configuration function.
     * @brief Sets the PWM module's duty cycle.
     */
    void set_pwm_duty_cycle(uint16_t duty_cycle);

    /**
     * PWM max duty cycle getter.
     * @brief Returs the minimum value corresponding to a 100 percent duty cycle.
     * @returns Minimum value coressponding to a 100 percent duty cycle.
     */
    uint16_t get_max_pwm_duty_cycle_conf_value();

#ifdef __cplusplus
}
#endif

#endif // PWM_H

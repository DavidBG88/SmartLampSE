#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <xc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * PMW init function.
 * @brief Initializes the pwm module.
 * @warning Must be called before any other pwm function.
 * @warning The PWM module takes ownership of the Timer2.
 * @warning General Interrupt Enable flag must be enabled (INTCONbits.GIE = 1).
 * @warning Peripheral Interrupt Enable flag must be enabled (INTCONbits.PEIE = 1).
 */
void init_pwm();

/**
 * PWM timer2 interrupt handler.
 * @brief Timer2 interrupt needed for the PWM module to work.
 * @warning Must be added to the interrupt handler and called when the Timer2 interrupt flag is raised.
 * @warning General Interrupt Enable flag must be enabled.
 */
void pwm_tmr2_interrupt_handler();

// Equation 11-1 from the manual
// Period = (PR2 + 1) * 4 * TOSC * TMR2Preescale
/**
 * PWM period configuration function.
 * @brief Sets the PWM module's period according to the following formula [Period = (pr2 + 1) * 4 * TOSC * tmr2ckps].
 * @warning For more info on the period equation check out equation 11-1 on the microcontroller's datasheet.
 */
void set_pwm_period(uint8_t pr2, uint8_t tmr2ckps);

/**
 * PWM duty cycle configuration function.
 * @brief Sets the PWM module's duty cycle ratio according to the following equation [Ratio = duty_cycle / (4 * (PR2 + 1))].
 * @warning For more info on the duty cycle ratio equation check out equation 13-3 on the microcontroller's datasheet.
 */
void set_pwm_duty_cycle(uint16_t duty_cycle);

/**
 * PWM max duty cycle getter.
 * @brief Returs the minimum value corresponding to a 100 percent duty cycle.
 * @returns Minimum value coressponding to a 100 percent duty cycle.
 * @warning For more info on the duty cycle ratio equation check out equation 13-3 on the microcontroller's datasheet.
 */
uint16_t get_max_pwm_duty_cycle();

#ifdef __cplusplus
}
#endif

#endif  // PWM_H

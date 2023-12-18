#include "pwm_test.h"
#include "xc.h"
#include "pwm.h"

#define _XTAL_FREQ 20000000  // necessary for __delay_us

void pwm_test() {
    PWM_init();
    PWM_set_period(255, 0);

    uint16_t max_duty = PWM_get_max_duty_cycle();
    uint16_t duty_arr[] = {0, max_duty/4, max_duty/2, 3*max_duty/4, max_duty};
    const uint8_t duty_arr_len = sizeof(duty_arr) / sizeof(duty_arr[0]);

    
    while (1) {
        for (uint8_t i = 0; i < duty_arr_len; ++i) {
            PWM_set_duty_cycle(duty_arr[i]);
            __delay_ms(1000);
        }
    }
}
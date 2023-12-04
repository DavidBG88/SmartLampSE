#include "pwm_test.h"

#include "pwm.h"

void pwm_test() {
    const uint8_t duty_arr[] = {0, 64, 128, 192, 255};
    const uint8_t duty_arr_len = sizeof(duty_arr) / sizeof(duty_arr[0]);

    PWM_init();
    PWM_set_period(255, 0);

    while (1) {
        for (uint8_t i = 0; i < duty_arr_len; ++i) {
            PWM_set_duty_cycle(duty_arr[i]);
            __delay_ms(5000);
        }
    }
}
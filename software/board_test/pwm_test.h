#ifndef PWM_TEST_H
#define PWM_TEST_H

const uint8_t duty_arr[] = {0, 64, 128, 192, 255};

/**
 * The device produces a PWM signal with duty cycles {0, 64, 128, 192, 255}.
 * The duty cycle changes every 5 seconds.
 */
void pwm_test();

#endif  // PWM_TEST_H
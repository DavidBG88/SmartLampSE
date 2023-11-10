#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000 // necessary for __delay_us

#include <xc.h>
#include <stdio.h>

#include "pwm.h"

int main()
{
    init_pwm();
    set_pwm_duty_cycle_percentage(50);

    while (1)
    {
    }

    return (EXIT_SUCCESS);
}

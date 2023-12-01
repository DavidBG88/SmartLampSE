// See /opt/microchip/xc8/v{ver}/docs/chips/16f886.html
// for details on #pragma config

#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000 // necessary for __delay_us

#include <xc.h>
#include <stdio.h>
#include <i2c-v2.h>
#include <uart.h>

void init_portB() {
    TRISBbits.TRISB0 = 0; // Configure port RB0 as output
    //TRISBbits.TRISB1 = 0; // Configure port RB1 as output
    //TRISBbits.TRISB2 = 0; // Configure port RB2 as output
}

void init_tmr0_5ms() {
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b110;
    TMR0 = 61;
    INTCONbits.GIE = 1;
    INTCONbits.T0IE = 1;
}

void init_adc() {
    // Enable interrupts
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;

    ADCON0bits.ADCS = 0b10; // ADCS = 1.6 us at 20 MHz
    ADCON0bits.CHS = 0; // Set channel 0
    ADCON1bits.ADFM = 1; // Justify right
    ADCON1bits.VCFG0 = 0; // AVdd
    ADCON1bits.VCFG1 = 0; // AVdd
    ADCON0bits.ADON = 1; // Enable ADC
}

void read_adc() {
    // TAD = 1.6 us, Tinstrucc >> TAD
    ADCON0bits.GO = 1; // Start ADC conversion
}

void init_uart() {
    TXSTAbits.BRGH = 0;
    BAUDCTLbits.BRG16 = 0;

    // SPBRGH:SPBRG =
    SPBRGH = 0;
    SPBRG = 32;  // 9600 baud rate with 20MHz Clock

    TXSTAbits.SYNC = 0; /* Asynchronous */
    TXSTAbits.TX9 = 0; /* TX 8 data bit */
    RCSTAbits.RX9 = 0; /* RX 8 data bit */

    PIE1bits.TXIE = 0; /* Disable TX interrupt */
    PIE1bits.RCIE = 0; /* Disable RX interrupt */

    RCSTAbits.SPEN = 1; /* Serial port enable */

    TXSTAbits.TXEN = 0; /* Reset transmitter */
    TXSTAbits.TXEN = 1; /* Enable transmitter */
}

int16_t adc_val = 0;

void every_500ms() {
    read_adc();
    PORTB = adc_val >> 2;
    printf("%d\r\n", adc_val);
}

void main() {
    OSCCON = 0b00001000; // External cristal

    /*
     * init_adc();
    init_uart();
    init_tmr0_5ms();
    init_portB();
    */
    char ack;
    char dato, dato2, daton;
    
    i2c_start();
    ack = i2c_write(0x81);
    ack = 1;
    dato = i2c_read(ack);
    uart_puts(dato);
    dato2 = i2c_read(ack);
    uart_puts(dato2);
    ack = 0;
    daton = i2c_read(ack);
    uart_puts(daton);
    i2c_stop();

    while(1);
}

int8_t i = 0;

void __interrupt() int_routine() {
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;

        adc_val = 0 | (ADRESH << 8) | ADRESL;
    } else if (INTCONbits.T0IF) {
        TMR0 = 61;
        INTCONbits.T0IF = 0;

        // 200 veces 5 ms
        if (i == 100) {
            every_500ms();
            i = 0;
        }
        i += 1;
    }
}

void putch(char data) {
  //while(!PIR1bits.TXIF); // Para simulador
  while(!TXSTAbits.TRMT); // Para el microchip
  TXREG = data;
}
 

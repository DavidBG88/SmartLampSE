#include "light_spi_test.h"

#include "spi.h"
#include "APA102.h"
#include "xc.h"

#define _XTAL_FREQ 20000000  // necessary for __delay_us

void light_spi_test() {
    SPI_init();
    APA102_init(10);
    
    APA102_set_color(31, 255, 0, 0); // RED
    __delay_ms(1000);
    APA102_set_color(31, 0, 255, 0); // GREEN
    __delay_ms(1000);
    APA102_set_color(31, 0, 0, 255); // BLUE
    __delay_ms(1000);
    
    uint8_t r, g, b;
    r = g = b = 0;
    
    while(1) {
        APA102_set_color(31, r, g, b);
        
        r += 2;
        g += 3;
        b += 5;
        __delay_ms(100);
    }
    
}
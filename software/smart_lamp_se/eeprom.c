#include "eeprom.h"

#include <xc.h>

uint8_t EEPROM_read(uint8_t address) {
    EEADR = address;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    return EEDAT;
}

bool EEPROM_write(uint8_t address, uint8_t data) {
    EEADR = address;
    EEDAT = data;

    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;

    uint8_t prev_gie = INTCONbits.GIE;

    do {
        /* To understand this loop, check the datasheet
        eeprom write example and aplication notice AN576 */
        INTCONbits.GIE = 0;
    } while (!INTCONbits.GIE);

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    INTCONbits.GIE = prev_gie;

    // Wait for write
    while (!PIR2bits.EEIF) {}
    PIR2bits.EEIF = 0;

    EECON1bits.WREN = 0;

    return data == EEPROM_read(address);
}
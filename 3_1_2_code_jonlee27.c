/* Name: 3_1_2_code_jonlee27.c
 * Author: Jonathan Lee
 * Description: Sub-routines for Lab 3.1.2.
 */

#include "MEAM_general.h"  // include resources from MEAM_general.h file

/**
 * setup ADC port
 */
void setup_ADC(unsigned int pin) {
    // first time setup tasks
    static int not_first_setup;
    if (!not_first_setup) {
        set(ADMUX, REFS0);  // use 5V / Vcc for Vref

        // set ADC clock pre-scaler
        set(ADCSRA, ADPS0);
        set(ADCSRA, ADPS1);
        set(ADCSRA, ADPS2);
    }

    // disable ADC pin digital input
    if (pin < 8) {
        set(DIDR0, pin);
    } else {
        set(DIDR2, pin - 8);
    }

    // select ADC channel
    if (pin > 7) {
        set(ADCSRB, MUX5);
        pin -= 8;
    } else {
        clear(ADCSRB, MUX5);
    }
    if (pin > 3) {
        set(ADMUX, MUX2);
        pin -= 4;
    } else {
        clear(ADMUX, MUX2);
    }
    if (pin > 1) {
        set(ADMUX, MUX1);
        pin -= 2;
    } else {
        clear(ADMUX, MUX1);
    }
    if (pin > 0) {
        set(ADMUX, MUX0);
        pin -= 1;
    } else {
        clear(ADMUX, MUX0);
    }

    set(ADCSRA, ADEN);  // enable ADC
    not_first_setup = 1;  // set not first setup to be true
}

/**
 * read ADC port
 * return: ADC reading
 */
unsigned int read_ADC() {
    set(ADCSRA, ADSC);                 // start conversion
    while(!bit_is_set(ADCSRA, ADIF));  // wait for conversion to complete
    set(ADCSRA, ADIF);                 // clear conversion complete flag
    return ADC;
}
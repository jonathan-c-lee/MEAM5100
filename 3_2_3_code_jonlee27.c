/* Name: 3_2_3_code_jonlee27.c
 * Author: Jonathan Lee
 * Description: Code for Lab 3.2.3.
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

/**
 * convert ADC reading to turret servo input
 * return: turret servo input
 */
unsigned int ADC_to_turret_servo(unsigned long adc) {
    unsigned int servo = 275 + (adc - 170) * 280 / 682;
    return servo;
}

/**
 * convert ADC reading to barrel servo input
 * return: barrel servo input
 */
unsigned int ADC_to_barrel_servo(unsigned long adc) {
    unsigned int servo = 275 + (adc - 170) * 360 / 682;
    return servo;
}

int main(void) {
    _clockdivide(0);  // set clock speed to 16 MHz
    set(DDRB, 5);     // set OC1A / PB5 pin to output
    set(DDRB, 6);     // set OC1B / PB6 pin to output

    // set Timer 1 pre-scaler to 64
    set(TCCR1B, CS10);
    set(TCCR1B, CS11);

    // set Mode 14, ICR1 is 5000 for 50Hz signal and < 1 degree resolution
    set(TCCR1A, WGM11);
    set(TCCR1B, WGM12);
    set(TCCR1B, WGM13);
    ICR1 = 5000;

    set(TCCR1A, COM1A1);  // clear OC1A at OCR1A, set at rollover
    set(TCCR1A, COM1B1);  // clear OC1B at OCR1B, set at rollover

    // center servos at start
    OCR1A = 415;
    OCR1B = 415;

    // main infinite loop
    for (;;) {
        // read turret ADC and set servo angle
        setup_ADC(1);
        unsigned int turret = ADC_to_turret_servo(read_ADC());

        // make sure values stay within acceptable range for servo
        if (turret < 275) {
            OCR1A = 275;
        } else if (turret > 555) {
            OCR1A = 555;
        } else {
            OCR1A = turret;
        }


        // read barrel ADC and set servo angle
        setup_ADC(4);
        unsigned int barrel = ADC_to_barrel_servo(read_ADC());

        // make sure values stay within acceptable range for servo
        if (barrel < 415) {
            OCR1B = 415;
        } else {
            OCR1B = barrel;
        }
    }
}
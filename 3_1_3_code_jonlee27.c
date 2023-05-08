/* Name: 3_1_3_code_jonlee27.c
 * Author: Jonathan Lee
 * Description: Code for Lab 3.1.3.
 */

#include "MEAM_general.h"  // include resources from MEAM_general.h file
#include "m_usb.h"         // include resources from m_usb.h file

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

    set(ADCSRA, ADEN);    // enable ADC
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

int main(void) {
    m_usb_init();     // initialize serial port
    _clockdivide(0);  // set clock speed to 16 MHz

    // main infinite loop
    for (;;) {
        // get first ADC reading
        setup_ADC(1);
        int turret = read_ADC();
        int turret_angle = (long) (turret - 511) * 135 / 511;  // calculate angle
        m_usb_tx_string("Turret Reading: ");
        m_usb_tx_int(turret);
        m_usb_tx_string(", Turret Angle: ");
        m_usb_tx_int(turret_angle);
        m_usb_tx_string(" Degrees\n");

        // get second ADC reading
        setup_ADC(4);
        int barrel = read_ADC();
        int barrel_angle = (long) (barrel - 511) * 135 / 511;  // calculate angle
        m_usb_tx_string("Barrel Reading: ");
        m_usb_tx_int(barrel);
        m_usb_tx_string(", Barrel Angle: ");
        m_usb_tx_int(barrel_angle);
        m_usb_tx_string(" Degrees\n");

        _delay_ms(1000);  // wait 1 second
    }
}
/* Name: 2_1_3_code_jonlee27.c
 * Author: Jonathan Lee
 * Description: Code for MEAM5100 Lab 2.1.3
 */

#include "MEAM_general.h"  // include resources from MEAM_general.h file
#include "m_usb.h"         // include resources from m_usb.h file

int main(void) {
    m_usb_init();       // initialize serial port
    _clockdivide(1);    // set clock speed to 8MHz
    clear(DDRC, 7);     // set port C7 as input
    set(TCCR3B, CS30);  // activate Timer 3
    set(TCCR3B, CS31);  // set Timer 3 pre-scaler to 64
    TCNT3 = 0;          // initialize 16-bit timer

    // main infinite loop
    for (;;) {
        set(TIFR3, ICF3);                     // clear flag
        set(TCCR3B, ICES3);                   // set input capture to look for rising edge
        while(!bit_is_set(TIFR3, ICF3));      // wait for button press
        unsigned int press = ICR3;            // store press time
        set(TIFR3, ICF3);                     // clear flag
        clear(TCCR3B, ICES3);                 // set input capture to look for falling edge
        while(!bit_is_set(TIFR3, ICF3));      // wait for button release
        unsigned int release = ICR3;          // store release time
        set(TIFR3, ICF3);                     // clear flag
        unsigned int diff = release - press;  // time in increments
        unsigned int time = 0.008 * diff;     // time in ms
        m_usb_tx_uint(time);                  // print time
        m_usb_tx_string(" ms\n");             // print units and new line
        _delay_ms(3);                         // delay to ensure print
    }
    return 0;
}
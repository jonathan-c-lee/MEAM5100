/* Name: 2_3_2_code_jonlee27.c
 * Author: Jonathan Lee
 * Description: Code for MEAM5100 Lab 2.3.2
 */

#include "MEAM_general.h"  // include resources from MEAM_general.h file

/**
 * calculate period of signal
 * return: period in timer ticks
 */
unsigned int find_period() {
    static unsigned int prev_time;    // store time of previous rising edge
    unsigned int start_time = TCNT3;  // store start time

    set(TIFR3, ICF3);  // clear flag

    // wait for rising edge, exit if it takes too long
    while (!bit_is_set(TIFR3, ICF3)) {
        if (TCNT3 - start_time > 30000) {
            return 0;
        }
    }
    unsigned int curr_time = ICR3;              // store time
    unsigned int diff = curr_time - prev_time;  // calculate time between edges
    prev_time = curr_time;                      // update previous time
    return diff;
}

int main(void) {
    _clockdivide(0);     // set clock speed to 16MHz
    clear(DDRC, 7);      // set port C7 as input
    set(DDRB, 7);        // set port B6 as output
    set(DDRB, 6);        // set port B7 as output
    set(TCCR3B, CS30);   // activate Timer 3
    set(TCCR3B, CS31);   // set Timer 3 pre-scaler to 64
    set(TCCR3B, ICES3);  // set input capture to look for rising edge
    TCNT3 = 0;           // initialize 16-bit timer

    // main infinite loop
    for (;;) {
        // match period to a frequency and turn on matching LED
        unsigned int period = find_period();
        if (period > 356 && period < 359) {
            // frequency is 700 Hz
            clear(PORTB, 7);
            set(PORTB, 6);
        } else if (period > 10700 && period < 11200) {
            // frequency is 23 Hz
            clear(PORTB, 6);
            set(PORTB, 7);
        } else {
            clear(PORTB, 7);
            clear(PORTB, 6);
        }
    }
    return 0;
}
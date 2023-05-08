/* Name: 2_1_1_code_jonlee27.c
 * Author: Jonathan Lee
 * Description: Code for MEAM5100 Lab 2.1.1
 */

#include "MEAM_general.h"  // include resources from MEAM_general.h file

/**
 * check if PC7 input has changed and toggle PB7 output if change occurred
 */
void check_PC7() {
    static int prev_state;                // store previous state
    int pin_state = bit_is_set(PINC, 7);  // retrieve current state

    // toggle B7 if input has changed
    if (pin_state != prev_state) {
        toggle(PORTB, 7);
    }
    prev_state = pin_state;
}

int main(void) {
    _clockdivide(0);  // set clock speed to 16MHz
    set(DDRB, 7);     // set port B7 as output
    clear(DDRC, 7);   // set port C7 as input

    // main infinite loop
    for (;;) {
        check_PC7();
    }
    return 0;
}
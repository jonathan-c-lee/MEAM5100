/* Name: 2_1_EC_code_jonlee27.c
 * Author: Jonathan Lee
 * Description: Code for MEAM5100 Lab 2.1 Extra Credit
 */

#include "MEAM_general.h"  // include resources from MEAM_general.h file
#include "m_usb.h"         // include resources from m_usb.h file

/**
 * calculate time taken to press and release switch
 * return: time taken to press and release switch in ms
 */
int press_time() {
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
    return time;
}

int main(void) {
    m_usb_init();       // initialize serial port
    _clockdivide(1);    // set clock speed to 8MHz
    clear(DDRC, 7);     // set port C7 as input
    set(TCCR3B, CS30);  // activate Timer 3
    set(TCCR3B, CS31);  // set Timer 3 pre-scaler to 64
    TCNT3 = 0;          // initialize 16-bit timer

    // start prompt
    _delay_ms(5000);
    m_usb_tx_string("Get Ready To Press Button 5 Times\n");
    _delay_ms(500);
    m_usb_tx_string("Start In 3 Seconds\n");
    _delay_ms(500);
    m_usb_tx_uint(3);
    m_usb_tx_string("\n");
    _delay_ms(1000);
    m_usb_tx_uint(2);
    m_usb_tx_string("\n");
    _delay_ms(1000);
    m_usb_tx_uint(1);
    m_usb_tx_string("\n");
    _delay_ms(1000);
    m_usb_tx_string("Go!");
    m_usb_tx_string("\n");

    // main loop
    int i;
    unsigned int times = 0;
    for (i = 0; i < 5; i++) {
        times += press_time();  // add each time
    }
    times /= 5;  // calculate average time

    // print average time
    m_usb_tx_string("Average Time:\n");
    m_usb_tx_uint(times);
    m_usb_tx_string(" ms\n");
    _delay_ms(10);
    return 0;
}
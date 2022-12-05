/*
 * File:   TimeDelay.c
 * Author: sadiakhandaker
 *
 * Created on November 26, 2022, 6:27 PM
 */


#include "xc.h"

void delay_ms(uint16_t time_ms) {
    T2CONbits.TCS = 0; // select internal clock source (Fosc/2)
    T2CONbits.T32 = 0; // 16 bit timer
    T2CONbits.TSIDL = 0; // continue operation in idle mode
    IEC0bits.T2IE = 1; // enable timer 2 interrupt
    IPC1bits.T2IP = 0b111; // set timer 2 interrupt priority to 7 (highest)
    PR2 = time_ms << 4; // set period register to 16 bit value
    TMR2 = 0; // clear timer 2
    T2CONbits.TON = 1; // turn on timer 2
    Idle(); // enter idle mode
}





void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF=0; //Clear timer 2 interrupt flag
    T2CONbits.TON=0; //Turn off timer 2
}



/*
 * Project: Driver Project 2, Basic GPIO Control using C Programming and Hardware
 * File:   main.c
 * Authors: Sadia Khandaker, Abdullah Khubaib, Ethan Kerr
 * Lab Section: B03
 * Group: Group 11
 *
 * Created on September 29, 2022, 6:32 PM
 */


#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

/*
Using the PIC 24F, you will design a simple IO controller to test out the hardware kit provided.
Design a state machine to turn on, turn off and blink a LED connected to port RB8 based on the push buttons
(PBs) connected to the input ports RA2, RA4 and RB4 as shown in the schematic in the lecture slide ?HW
and IO Control.pdf?. PB1, PB2 and PB3 represent push buttons connected to ports RA2, RA4 and RB4
respectively. The state machine should operate as follows:

User input(s)                                 |     Output(s)
------------------------------------------------------------------------------------------------------------------------
While PB1 is pressed                          |     LED blinks at approximately 1 sec interval (1 sec on and 1 sec off)
While PB2 is pressed                          |     LED blinks at approximately 2 sec interval (2sec on and 2 sec off)
While PB3 is pressed                          |     LED blinks at approximately 3 sec interval (3sec on and 3 sec off)
While 2 or more PBs are pressed together      |     LED stays on without blinking
------------------------------------------------------------------------------------------------------------------------
 */

int main(void) {

    AD1PCFG = 0xFFFF;  // Turn all analog pins to digital

    TRISBbits.TRISB8 = 0; // Set GPIO RB8 as digital output, which is connected to LED

    TRISAbits.TRISA2 = 1; // Set GPIO RA2 as digital input, which is connected to PB1
    TRISAbits.TRISA4 = 1; // Set GPIO RA4 as digital input, which is connected to PB2
    TRISBbits.TRISB4 = 1; // Set GPIO RB4 as digital input, which is connected to PB3

    CNPU1bits.CN0PUE = 1; // Enable pull-up resistor for RA4/CN0
    CNPU2bits.CN30PUE = 1; // Enable pull-up resistor for RA2/CN30
    CNPU1bits.CN1PUE = 1; // Enable pull-up resistor for RB4/CN1

    return 0;
}
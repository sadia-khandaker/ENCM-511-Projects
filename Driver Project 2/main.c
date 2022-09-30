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

// PRE-PROCESSOR DIRECTIVES
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled

// MACROS
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   // set sleep mode
#define Idle() {__asm__ volatile ("pwrsav #1");}
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}

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


void initIO(void) { // Initialize IO ports
    AD1PCFG = 0xFFFF; // Set all analog pins to digital
    TRISBbits.TRISB8 = 0; // Set GPIO RB8 as digital output, which is connected to LED
    TRISAbits.TRISA2 = 1; // Set GPIO RA2 as digital input, which is connected to PB1
    TRISAbits.TRISA4 = 1; // Set GPIO RA4 as digital input, which is connected to PB2
    TRISBbits.TRISB4 = 1; // Set GPIO RB4 as digital input, which is connected to PB3
    CNPU1bits.CN0PUE = 1; // Enable pull-up resistor for RA4/CN0
    CNPU2bits.CN30PUE = 1; // Enable pull-up resistor for RA2/CN30
    CNPU1bits.CN1PUE = 1; // Enable pull-up resistor for RB4/CN1
    CNEN1bits.CN0IE = 1; // Enable CN0 interrupt for port RA4 (PB2)
    CNEN2bits.CN30IE = 1; // Enable CN30 interrupt for port RA2 (PB1)
    CNEN1bits.CN1IE = 1; // Enable CN1 interrupt for port RB4 (PB3)
    IFS1bits.CNIF = 0; // Clear CN interrupt flag bit
    IEC1bits.CNIE = 1; // Enable CN interrupt enable bit
    IPC4bits.CNIP = 7; // Set CN interrupt priority to 7 (highest priority)
}

//clkval = 8 for 8MHz;
//clkval = 500 for 500kHz;
//clkval = 32 for 32kHz;
void set_clock_frequency(unsigned int clkval) { // Set clock frequency to clkval
    char COSCNOSC;
    if (clkval == 8)  //8MHz
    {
        COSCNOSC = 0x00;
    } else if (clkval == 500) // 500 kHz
    {
        COSCNOSC = 0x66;
    } else if (clkval == 32) //32 kHz
    {
        COSCNOSC = 0x55;
    } else // default 32 kHz
    {
        COSCNOSC = 0x55;
    }
    // Switch clock to 500 kHz
    SRbits.IPL = 7;  //Disable interrupts
    CLKDIVbits.RCDIV = 0;  // CLK division = 0
    __builtin_write_OSCCONH(COSCNOSC);   // (0x00) for 8MHz; (0x66) for 500kHz; (0x55) for 32kHz;
    __builtin_write_OSCCONL(0x01);
    OSCCONbits.OSWEN = 1;
    while (OSCCONbits.OSWEN == 1) {}
    SRbits.IPL = 0;  //enable interrupts
}
// TODO: Add interrupt service routine for CN interrupt without using timer functions.
//void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) { // Interrupt service routine for CN interrupt, this function calls the timer functions
//    IFS1bits.CNIF = 0; // Clear CN interrupt flag bit
//    if (PORTAbits.RA2 == 0) { // If PB1 is pressed
//        LATBbits.LATB8 = 1; // Turn on LED
//        __delay_ms(1000); // Delay for 1 second
//        LATBbits.LATB8 = 0; // Turn off LED
//        __delay_ms(1000); // Delay for 1 second
//    } else if (PORTAbits.RA4 == 0) { // If PB2 is pressed
//        LATBbits.LATB8 = 1; // Turn on LED
//        __delay_ms(2000); // Delay for 2 seconds
//        LATBbits.LATB8 = 0; // Turn off LED
//        __delay_ms(2000); // Delay for 2 seconds
//    } else if (PORTBbits.RB4 == 0) { // If PB3 is pressed
//        LATBbits.LATB8 = 1; // Turn on LED
//        __delay_ms(3000); // Delay for 3 seconds
//        LATBbits.LATB8 = 0; // Turn off LED
//        __delay_ms(3000); // Delay for 3 seconds
//    } else { // If 2 or more PBs are pressed together
//        LATBbits.LATB8 = 1; // Turn on LED
//    }
//}

void toggleLED(void) { // Toggle LED
    LATBbits.LATB8 = ~LATBbits.LATB8; // Toggle LED, so if LED is on, turn it off, and vice versa
}

int main(void) {
    initIO(); // Initialize IO ports

    return 0;
}

/**
 * 
 * File: TimeDelay.c
 * 
 */


#include "xc.h"
#include "TimeDelay.h"


// TO DO: Check if either this delay function or our delay function from Driver Project 3 works

// PROVIDED CODE FROM DR. VYAS

void delay_ms(uint16_t time_ms, uint8_t idle_on) {
    NewClk(8);
    //T2CON config
    T2CONbits.TSIDL = 0; //operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0; // use internal clock
    // T2CONbits.TGATE = 0;


    // Timer 2 interrupt config
    IPC1bits.T2IP = 2; //7 is highest and 1 is lowest priority
    IEC0bits.T2IE = 1; //enable timer interrupt
    IFS0bits.T2IF = 0; // Clear timer 2 flaf

    PR2 = time_ms << 4; //After PR2 simplification
    TMR2 = 0;
    T2CONbits.TON = 1; //start timer

    if (idle_on == 1) {
        Idle();
    }
    T2CONbits.TON = 0; // Stop timer
    NewClk(32);
    return;
}


// FROM OUR DRIVER PROJECT 3 
//void delay_ms(uint16_t time_ms, uint8_t idle_on) {
//    NewClk(8); // Change to 8 MHz clock before using timer
//    T2CONbits.TON = 0;  // Turn off timer
//    T2CONbits.TCS = 0;  // Select internal instruction cycle clock (FOSC/4)
//    T2CONbits.TGATE = 0;  // Disable Gated Timer mode
//    T2CONbits.TCKPS = 0b00;  // Select 1:1 Prescaler
//    TMR2 = 0x00;  // Clear timer register
//    PR2 = 16 * time_ms;  // Load the period value
//    IFS0bits.T2IF = 0;  // Clear Timer2 Interrupt Flag
//    IEC0bits.T2IE = 1;  // Enable Timer2 interrupt
//    T2CONbits.TON = 1;  // Start Timer
//    if (idle_on == 1) { // If idle_on = 1, put MCU in idle mode
//        Idle();
//    } else { // If idle_on = 0, put MCU in sleep mode
//        Sleep();
//    }
//    NewClk(32); // Change back to 32 kHz clock after using timer to save power
//}

// Used in HW IO control Driver project 3

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    // TMR2flag = 1; // optional global variable created by user
    return;
}
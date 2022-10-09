//
// Created by Sadia Khandaker on 2022-10-05.
//

#include "TimeDelay.h"
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// delay_ms(time_ms) ? implements the delay functions used to time the LED blinks. time_ms is the user
////specified time delay in milliseconds. Your function is only expected to handle delays of whole numbers
////(e.g. 1 ms, 5 ms, 2504 ms) and not floating point numbers (e.g. 1.2 ms, 5.5 ms, 2504.8ms) .

// create your own __delay_ms() function using only timers and interrupts
void delay_ms(unsigned int time_ms){
    T1CONbits.TON = 0; // Stop timer
    T1CONbits.TCS = 0; // Select internal instruction cycle clock
    T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR1 = 0x00; // Clear timer register
    PR1 = 0x9C3F; // Load the period value
    T1CONbits.TON = 1; // Start timer
    while(time_ms != 0){
        while(IFS0bits.T1IF == 0); // Wait for Timer1 to overflow
        IFS0bits.T1IF = 0; // Clear Timer1 interrupt flag
        time_ms--;
    }
    T1CONbits.TON = 0; // Stop timer


}




//void delay_ms(unsigned int time_ms){
//    // set up timers
//    T2CONbits.TCKPS = 0b11; // set prescaler to 256
//    TMR2 = 0; // clear timer
//    PR2 = 6249; // set period register
//    T2CONbits.ON = 1; // turn on timer
//    // set up interrupts
//    IFS0bits.T2IF = 0; // clear interrupt flag
//    IEC0bits.T2IE = 1; // enable interrupt
//    // set up timer
//    while (time_ms > 0) {
//        while (IFS0bits.T2IF == 0) {
//            // wait for interrupt flag to be set
//        }
//        IFS0bits.T2IF = 0; // clear interrupt flag
//        time_ms--; // decrement time
//    }
//    T2CONbits.ON = 0; // turn off timer
//
//
//}

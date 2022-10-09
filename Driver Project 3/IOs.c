/*
 * File:   IOs.c
 * Author: sadiakhandaker
 *
 * Created on October 5, 2022, 1:36 PM
 */

#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>


//Using the PIC 24F16KA101 and Timer interrupts, you will design a simple IO controller to test out Timers
//        and Interrupts. Design a state machine to turn on, turn off and blink a LED connected to port RB8 based
//        on the push buttons (PBs) connected to the input ports RA2, RA4 and RB4 as shown in the schematic in
//the lecture slide ?HW and IO Control.pdf?. PB1, PB2 and PB3 represent push buttons connected to ports
//        RA2, RA4 and RB4 respectively. The state machine should operate as follows:
//User input(s) Output(s)
//While PB1 is pressed LED blinks at approx. 1 sec intervals (1 sec on and 1 sec off)
//While PB2 is pressed LED blinks at approx. 2 sec intervals (2 sec on and 2 sec off)
//While PB3 is pressed LED blinks at approx. 3 sec intervals (3 sec on and 3 sec off)
//While PB1 and PB2 are pressed LED blinks for 1 msec or smallest delay that your delay
//        function can produce. LEDs blinking at such small time
//intervals are not visible to the human eye. Verify this delay
//        by probing the RB8 pin connected to the LED with your
//ADALM oscilloscope.
//No PBs pressed LED stays off
//
//Additional info:
//Implement the above controller using the hardware kit and your code, which will be designed using basic
//ANSI C commands and Timer interrupts. Use of polling and built in __delay () functions instead of timers
//and interrupts will lose points.
//IOinit() ? initializes the IO ports and placed in IOs.c
//IOcheck() ? implements the IO checks and LED blinking functions and placed in IOs.c
//delay_ms(time_ms) ? implements the delay functions used to time the LED blinks. time_ms is the user
//specified time delay in milliseconds. Your function is only expected to handle delays of whole numbers
//(e.g. 1 ms, 5 ms, 2504 ms) and not floating point numbers (e.g. 1.2 ms, 5.5 ms, 2504.8ms) . Place timer
//related functions in source file TimeDelay.c
//main() ? Used to call IOinit() and IOcheck() and placed in main.c
//        Hint on generating delay cycles: Use timers, interrupts and if applicable clock switching for the delay
//function.
//Note: Port RA2 is one of those exceptional ports that is also multiplexed to the input for an external
//oscillator and an analog input port. To be able to use it as a digital input with a pushbutton, it?s
//        multiplexed analog input has to be disabled by including the following line of code in your IOinit()
//function. We will revisit this multiplexing when we look at ADC converters in a couple of weeks.
//AD1PCFG = 0xFFFF; // Turn all analog pins as digital
// CANNOT USE POLLING, MUST USE INTERRUPTS
void IOinit() {

    AD1PCFG = 0xFFFF; // Set all analog pins to digital

    TRISBbits.TRISB8 = 0; // Set GPIO RB8 as digital output, which is connected to LED

    TRISAbits.TRISA2 = 1; // Set GPIO RA2 as digital input, which is connected to PB1
    TRISAbits.TRISA4 = 1; // Set GPIO RA4 as digital input, which is connected to PB2
    TRISBbits.TRISB4 = 1; // Set GPIO RB4 as digital input, which is connected to PB3

    CNPU1bits.CN0PUE = 1; // Enable pull-up resistor for RA4/CN0
    CNPU2bits.CN30PUE = 1; // Enable pull-up resistor for RA2/CN30
    CNPU1bits.CN1PUE = 1; // Enable pull-up resistor for RB4/CN1

    CNEN1bits.CN0IE = 1; // Enable CN0 interrupt
    CNEN2bits.CN30IE = 1; // Enable CN30 interrupt
    CNEN1bits.CN1IE = 1; // Enable CN1 interrupt

    IFS1bits.CNIF = 0; // Clear CN interrupt flag
    IEC1bits.CNIE = 1; // Enable CN interrupt

}

void IOcheck() {
    if (PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) { // PB1 is pressed
        LATBbits.LATB8 = 1; // Turn on LED
        delay_ms(1000); // Delay 1 sec
        LATBbits.LATB8 = 0; // Turn off LED
        delay_ms(1000); // Delay 1 sec
    } else if (PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1) { // PB2 is pressed
        LATBbits.LATB8 = 1; // Turn on LED
        delay_ms(2000); // Delay 2 sec
        LATBbits.LATB8 = 0; // Turn off LED
        delay_ms(2000); // Delay 2 sec
    } else if (PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0) { // PB3 is pressed
        LATBbits.LATB8 = 1; // Turn on LED
        delay_ms(3000); // Delay 3 sec
        LATBbits.LATB8 = 0; // Turn off LED
        delay_ms(3000); // Delay 3 sec
    } else if (PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1) { // PB1 and PB2 are pressed
        LATBbits.LATB8 = 1; // Turn on LED
        delay_ms(1); // Delay 1 ms
        LATBbits.LATB8 = 0; // Turn off LED
        delay_ms(1); // Delay 1 ms
    } else { // No PBs are pressed
        LATBbits.LATB8 = 0; // Turn off LED
    }

}


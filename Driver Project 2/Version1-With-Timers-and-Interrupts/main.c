/*
 * Project: Driver Project 2, Basic GPIO Control using C Programming and Hardware
 * File:   main.c
 * Authors: Sadia Khandaker, Abdullah Khubaib, Ethan Kerr
 * Lab Section: B03
 * Group: Group 11
 *
 * Created on September 29, 2022, 6:32 PM
 */

//// HEADER FILES ////
#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <libpic30.h>
#include <stdlib.h>


//// CONFIGURATION BITS - PRE-PROCESSOR DIRECTIVES ////

// Code protection
#pragma config BSS = OFF // Boot segment code protect disabled
#pragma config BWRP = OFF // Boot sengment flash write protection off
#pragma config GCP = OFF // general segment code protecion off
#pragma config GWRP = OFF

// CLOCK CONTROL
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO.
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled

// WDT
#pragma config FWDTEN = OFF // WDT is off
#pragma config WINDIS = OFF // STANDARD WDT/. Applicable if WDT is on
#pragma config FWPSA = PR32 // WDT is selected uses prescaler of 32
#pragma config WDTPS = PS1 // WDT postscler is 1 if WDT selected

//MCLR/RA5 CONTROL
#pragma config MCLRE = OFF // RA5 pin configured as input, MCLR reset on RA5 diabled

//BOR  - FPOR Register
#pragma config BORV = LPBOR // LPBOR value=2V is BOR enabled
#pragma config BOREN = BOR0 // BOR controlled using SBOREN bit
#pragma config PWRTEN = OFF // Powerup timer disabled
#pragma config I2C1SEL = PRI // Default location for SCL1/SDA1 pin

//JTAG FICD Register
#pragma config BKBUG = OFF // Background Debugger functions disabled
#pragma config ICS = PGx2 // PGC2 (pin2) & PGD2 (pin3) are used to connect PICKIT3 debugger

// Deep Sleep RTCC WDT
#pragma config DSWDTEN = OFF // Deep Sleep WDT is disabled
#pragma config DSBOREN = OFF // Deep Sleep BOR is disabled
#pragma config RTCOSC = LPRC// RTCC uses LPRC 32kHz for clock
#pragma config DSWDTOSC = LPRC // DeepSleep WDT uses Lo Power RC clk
#pragma config DSWDTPS = DSWDTPS7 // DSWDT postscaler set to 32768

//// MACROS ////
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


void delay_ms(unsigned int ms) {
    // The default clock frequency is 8MHz. The
    //program counter increments from one instruction to the next at every other clock cycle (i.e. 4
    //MHz). The number of clock cycles needed per instruction varies between 2 and 20 for different
    //types of instructions with the free C compiler use
    T2CONbits.TON = 0; // Disable Timer2
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TGATE = 0; // Disable Gated Timer mode
    T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR2 = 0x00; // Clear timer register
    PR2 = 2000; // Load the period value
    IFS0bits.T2IF = 0; // Clear Timer2 interrupt status flag
    T2CONbits.TON = 1; // Start Timer
    while (ms > 0) {
        while (IFS0bits.T2IF == 0); // Wait for Timer2 to overflow
        IFS0bits.T2IF = 0; // Clear Timer2 interrupt status flag
        ms--;
    }
    T2CONbits.TON = 0; // Stop Timer
    Idle();
}

void checkIO(){
    if (PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 1) { // PB1 is pressed
        LATBbits.LATB8 = 1; // Turn on LED
        delay_ms(1000); // Wait 1 sec
        LATBbits.LATB8 = 0; // Turn off LED
        delay_ms(1000); // Wait 1 sec
    } else if (PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1) { // PB2 is pressed
        LATBbits.LATB8 = 1; // Turn on LED
        delay_ms(2000); // Wait 2 sec
        LATBbits.LATB8 = 0; // Turn off LED
        delay_ms(2000); // Wait 2 sec
    } else if (PORTAbits.RA2 == 1 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0) { // PB3 is pressed
        LATBbits.LATB8 = 1; // Turn on LED
        delay_ms(3000); // Wait 3 sec
        LATBbits.LATB8 = 0; // Turn off LED
        delay_ms(3000); // Wait 3 sec
    } else if (PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 1) { // PB1 and PB2 are pressed
        LATBbits.LATB8 = 1; // Turn on LED
    } else if (PORTAbits.RA2 == 0 && PORTAbits.RA4 == 1 && PORTBbits.RB4 == 0) { // PB1 and PB3 are pressed
        LATBbits.LATB8 = 1; // Turn on LED
    } else if (PORTAbits.RA2 == 1 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0) { // PB2 and PB3 are pressed
        LATBbits.LATB8 = 1; // Turn on LED
    } else
        LATBbits.LATB8 = 0; // Turn off LED
        Idle(); // Puts the processor in idle mode
}


void __attribute__((interrupt, no_auto_psv)) CNInterrupt(void) {
    IFS1bits.CNIF = 0; // Clear interrupt flag
    checkIO();
}

int main(void) {
    set_clock_frequency(8);
    initIO();
    while(1) {
        checkIO();
    }
    return 0;
}

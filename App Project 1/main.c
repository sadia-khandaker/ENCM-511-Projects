/*
 * File:   main.c
 * Author: sadiakhandaker
 *
 * Created on November 17, 2022, 2:16 PM
 */


/*
 * Countdown Timer Application
 * 
 * The app will use push buttons (PBs) connected to input ports RA2, RA4 and RB4.
 * Push buttons PB1, PB2, and PB3 are connected to ports RA2, RA4, and RB4, respectively.
 * The count down timer should operate as follows:
 * 
 * Possible User Inputs and Outputs:
 * ---------------------------------
 * Case 1: While PB1 is pressed
 * While PB1 is pressed, it sets the number of minutes to be counted down from. 
 * When PB1 is pressed, the minute count increments by one minute from 0 to 59. The minute 
 * count should be displayed on the PC terminal as shown below. The increments should occur
 * at a slow enough rate that a user can stop at a desired minute count, such as the 52nd
 * minute shown below: 
 * 
 * 52m : 00s
 * 
 * It clears any ALARM messages from previous countdowns.
 * 
 * Case 2: While PB2 is pressed
 * While PB2 is pressed, it sets the number of seconds to be counted down from.
 * When PB2 is pressed, the seconds count increases by one second from 0 to 59. 
 * The second count should be displayed on the PC terminal as shown below. The 
 * increments should occur at a slow enough rate that a user can stop at a desired 
 * minute count, such as the 45th second shown below.
 * 
 * 52m : 45s
 * 
 * It clears any ALARM messages from previous countdowns.
 * 
 * Case 3: Short presses (< 3sec) on PB3
 * When there are short presses (3 seconds or less) on PB3, a short
 * PB3 press starts or pauses the count down if the minutes and seconds are
 * correctly set.
 * 
 * Case 4: Long press (> 3sec) on PB3
 * When there are long presses (more than three seconds), a long press on PB3
 * on the PC terminal stops the count-down and resets the timer to 0, as 
 * shown below. 
 * 
 * During countdown: The PC terminal should be updated every second to display
 * the decremented minutes and/or seconds, for example.
 * 
 * 05m : 08s
 * 
 * The LED should blink every 1 second.
 * 
 * 
 * When the countdown has ended: The PC terminal should be updated to show
 * the following information.
 * 
 * 00m : 00s -- ALARM
 * 
 * LED stays ON
 * 
 */

// MPLAB header libraries
#include <xc.h>

#include <libpic30.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>


// User header files

#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"

// PRE-PROCESSOR DIRECTIVES - CONFIGURATION BITS

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
#pragma config MCLRE = ON // RA5 pin configured as input, MCLR reset on RA5 diabled

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

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //
// Global variables
//uint8_t CNflag = 0;
//uint16_t i = 0;

void main(void) {
    NewClk(32);
    IOinit(); // Initialize IOs for low-power wake-up
    InitUART2(); // Initialize UART settings and enable UART module
    IOcheck();
    while (1) {
         // Check if any IOs are active
        Idle(); // Put MCU in idle mode
    }
    
    return;
  
}


//// SAMPLE MAIN
//void main(void) {
//    //Clock output on REFO/RB15 - Testing purposes only
//     TRISBbits.TRISB15 = 0;  // Set RB15 as output for REFO
//     REFOCONbits.ROEN = 1; // Ref oscillator is enabled
//     REFOCONbits.ROSSLP = 0; // Ref oscillator is disabled in sleep
//     REFOCONbits.ROSEL = 0; // Output base clk showing clock switching
//     REFOCONbits.RODIV = 0b0000;
//     
//    // Change Clock of MCU - Adjust UART Baud rate on PC accordingly
//     NewClk(8); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
//     
//   // Initialize IOs for low-power wake-up
//    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
//    IOinit();         // enables IO and CN interrupts on Push buttons
//    InitUART2();      //Initialize UART settings and enable UART module
//    
//    while(1)
//    {
//        
//        /*-- Display Commands --*/      
//        XmitUART2('G', 5);    // Xmits char 'G' 5 times via UART2 
//        XmitUART2('\n',1);    // Xmits New line ASCII command via UART2    
//        XmitUART2('\r',1);    // Xmits Carriage Return ASCII command via UART2
//        
//        Disp2String("\r Hello U of C - Today is day ");
//        Disp2Dec(i);
//        Disp2Hex(i);
//        i=i+1;
//        XmitUART2('\n',1);    // Xmits New line ASCII command via UART2    
//        XmitUART2('\r',1);    // Xmits Carriage Return ASCII command via UART2
//        
//        Disp2String("\r PORTA is: ");   
//        Disp2Hex(PORTA);        //Xmits PORTA values via UART in 16bit Hex form
//        XmitUART2('\n',1);    // Xmits New line ASCII command via UART2    
//        XmitUART2('\r',1);    // Xmits Carriage Return ASCII command via UART2
//        
//        double f;
//        char str[10];
//        f= -15.5678;
//        sprintf(str, "%1.5f", f); // translates -15.5678 into array of char called str[]]
//        Disp2String(str);         //Display str[]
//        XmitUART2('\n',1);        // Xmits New line ASCII command via UART2
//        XmitUART2('\r',1);        // Xmits Carriage Return ASCII command via UART2
//        
//        
////        signed int d;
////        char str[10];
////        d =-58;                       
////        sprintf(str, "%1.0d", d);     // translates -15.5678 into array of char called str[]    
////        Disp2String(str);             //Display str[]
////        XmitUART2('\n',1);            // Xmits New line ASCII command via UART2
////        XmitUART2('\r',1);            // Xmits Carriage Return ASCII command via UART2
//        
//        
//        LATBbits.LATB8 = 1;             //Turn on LED
////        Idle();                       // Put MCU in idle mode - OPTIONAL
//        
//    }
//    
//    return;
//}
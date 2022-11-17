/*
 * File:   main.c
 *
 * Driver Project 4: IO Interrupts, Timers and UART Display Drivers
 * Using the Microcontroller and the driver topics on IOs, Timers,
 * interrupts and UART display drivers covered so far, you will design
 * a simple IO controller to test out IO and Timer Interrupts with
 * display drivers. Design a state machine to turn on, turn off and
 * blink a LED connected to port RB8 based on the push buttons (PBs)
 * connected to the input ports RA2, RA4 and RB4.B1, PB2 and PB3
 * represent push buttons connected to ports RA2, RA4 and RB4 respectively.
 * The state machine should operate as follows:
 * While PB1 is pressed, LED blinks at approx. 0.5 sec intervals (0.5 sec on and 0.5 sec off)
 * "PB1 is pressed" is displayed on PC terminal.
 * While PB2 is pressed, LED blinks at approx. 2 sec intervals (2 sec on and 2 sec off)
 * "PB2 is pressed" is displayed on PC terminal
 * While PB3 is pressed, LED blinks at approx. 3 sec intervals (3 sec on and 3 sec off)
 * "PB3 is pressed" is displayed on PC terminal
 * While 2 PBs are pressed together, LED stays on without blinking
 * "PB_ and PB_ are pressed" is displayed on PC terminal. “
 * The blanks __ should display the corresponding PB numbers pressed
 * While all 3 PBs are pressed together, LED stays on without blinking
 * "All PBs pressed" is displayed on PC terminal
 * No PBs pressed, LED stays off "Nothing pressed" is displayed on the PC terminal
 *
 * Additional info:
 * 1. Implement the above controller using the hardware kit
 * and your code, which will be designed using basic ANSI C
 * commands; IO and Timer interrupts; and Display driver
 * functions provided.
 * 2. Use of polling and manufacturer-provided timer
 * functions instead of interrupts will lose points.
 * Function names: Students can use any convention when
 * naming functions or organizing code.
 * 3. Display instructions: All displays on the PC terminal
 * window should be on a single line. Note that display functions
 * carried out at 32 kHz (300 Baud) can affect timer delays. Your
 * code should account for such delays when producing delays
 * specified in the table above.
 * 4. Interrupts: Interrupt ISR names are provided in the
 * lecture slides. As specified in lecture, IO (CN interrupts) are
 * triggered on rising and falling edges and due to any debounce
 * effects of the push buttons. A debounced switch will result in
 * several hi to lo and lo to hi fluctuations at the Microcontroller
 * input before stabilizing to a steady and fixed voltage when the
 * switch is pressed. Your code should filter out any such effects.
 *
 * Note: Port RA2 is one of those exceptional ports that is also
 * multiplexed to the input for an external oscillator and an analog
 * input port. To be able to use it as a digital input with a pushbutton,
 * it’s multiplexed analog input has to be disabled by including the
 * following line of code in your IOinit() function. We will revisit
 * this multiplexing when we look at ADC converters in a couple of weeks.
 * AD1PCFG = 0xFFFF; // Turn all analog pins as digital

 */

// MPLAB header libraries
#include <xc.h>

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
uint8_t CNflag = 0;
uint16_t i = 0;

void main(void) {
    //NewClk(8);
    IOinit(); // Initialize IOs for low-power wake-up
    InitUART2(); // Initialize UART settings and enable UART module
    IOcheck();
    while (1) {
         // Check if any IOs are active
        Idle(); // Put MCU in idle mode
    }
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

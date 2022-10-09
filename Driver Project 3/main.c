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


// MACROS
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   // set sleep mode
#define Idle() {__asm__ volatile ("pwrsav #1");}
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}

#include "IOs.h"
#include "TimeDelay.h"


#include <stdio.h>

int main(void) {
    IOinit();
    while(1){
        IOcheck();
    }
    return 0;
    
}
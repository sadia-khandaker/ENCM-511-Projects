// ENCM 511: App Project 1
// File Name: IOs.c
// Lab Section: B03 
// Group: 11
// Group Members: Sadia Khandaker, Ethan Kerr, Abdullah Khubaib

#include <xc.h>
#include <time.h>
#include <string.h>
#include <libpic30.h>
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"


extern uint16_t min;
extern uint16_t sec;
extern uint16_t flag;

void format_time(void){
    char buffer[30];
    sprintf(buffer, "\r%02dm : %02ds",  min, sec);
    Disp2String(buffer);
  
}

void IOinit(void)
{
    
    AD1PCFG = 0xFFFF; // Set all analog pins to digital
    TRISBbits.TRISB8 = 0; // Make GPIO RB8 as a digital output
   
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output
    
    
    // PB1 PUSH BUTTON is connected to PORT RA2
    // PB2 PUSH BUTTON is connected to PORT RA4
    // PB3 PUSH BUTTON is connected to PORT RB4
  
    TRISAbits.TRISA4 = 1; // Makes GPIO RA4 as a digital input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1;  // enable CN on CN0
    
   
    TRISBbits.TRISB4 = 1; // Makes GPIO RB4 as a digital input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN1IE = 1;  // enable CN on CN1
    
 
    TRISAbits.TRISA2 = 1; // Makes GPIO RA2 as a digital input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN2bits.CN30IE = 1;  // enable CN on CN30
    
    //Interrupt settings
    IPC4bits.CNIP = 6; //7 is highest priority, 0 is disabled interrupt
    IFS1bits.CNIF = 0;		// clear IF flag
    IEC1bits.CNIE = 1; //Enable CN interrupts
    
    return;
}
#if 1
void IOcheck() {
    uint16_t counter = 0; // counter for PB3 press time
 
    while (PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1 && min <= 59)  //while only RA2 pb is pressed
    {
        min++;          // Increment minute count
        NewClk(8);      // Speed up clock just for display
        format_time(); 
        //XmitUART2('\r',1);        // Xmits Carriage Return ASCII command via UART2
        XmitUART2(' ', 9);
        delay_ms(500);  // 500 ms delay
        NewClk(32);     // speed up clock just for display
    }
    while (PORTAbits.RA2 == 1 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 0 && sec <= 59) // while only RA4, pb2 is pressed
    {
        NewClk(8);      // // Speed up clock just for display
        sec++;          // increment second count
        format_time();    // print current timer value
        //XmitUART2('\r',1);        // Xmits Carriage Return ASCII command via UART2
        XmitUART2(' ', 9); // Clear any previous ALARM messages 
        
        delay_ms(500);  // 500 ms delay for slower pace
        NewClk(32);     // slow down clock for delay and other tasks
    }
    while (PORTAbits.RA2 == 1 && PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1) // while RB4, pb3 is pressed
    {
        counter++;     // increment counter by 1 to track how long PB3 is pressed
        delay_ms(100);  // delay of 100 ms
    }
    
    
    // if pb3 been pressed for less than 3 seconds, 
    //pause or start the timer, and if pb3 been pressed 
    //for more than 3 seconds, stop the timer
    if ((counter < 30) && (counter != 0)) { 
        flag = !flag;           
    } else if (counter >= 30) {
        LATBbits.LATB8 = 0;     // turn off LED
        flag = 0;               // stop the timer
        min = 0;                // zero minute count
        sec = 0;                // zero second count
        NewClk(8);      // Speed up clock just for display
        format_time();            // print cleared timer
        //XmitUART2('\r',1);        // Xmits Carriage Return ASCII command via UART2
        XmitUART2(' ', 9);  // clear previous ALARM message
        NewClk(32);    // slow down clock for delay and other tasks
    }
    
    
    // While the countdown is still on, and the timer is not at 00:00, 
    // and PB3 is not pressed, keep counting down
    while (flag == 1 && (min != 0 || sec != 0) && (PORTBbits.RB4 != 0)) {
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;   // toggle LED
        NewClk(8);     // Speed up clock just for display
        format_time();                        // print current timer value
        NewClk(32);     // slow down clock for delay and other tasks
        if (sec == 0) { // if seconds is 0
            sec = 59;  // set seconds to 59
            min--;    // decrement minute by 1
        } else {
            sec--;   // decrement seconds by 1
        }
        delay_ms(100);
    }

    // If countdown has reached 00 m : 00 s
    if ((min == 0 && sec == 0) && flag == 1) {
        NewClk(8);                             // Speed up clock just for display
        format_time();                       // print the current timer value
        NewClk(32);                          // slow down clock for delay and other tasks
        Disp2String(" -- ALARM");            // print --ALARM 
        LATBbits.LATB8 = 1;                     // turn on LED until the timer is reset
        flag = 0;                               // set flag to zero because the timer is done
    }
    
    return;
}
#endif





/// Change of pin Interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
	IFS1bits.CNIF = 0;		// clear IF flag
    T2CONbits.TON = 0; // Disable timer
    IEC0bits.T2IE = 0; //Disable timer interrupt
    IOcheck();
	Nop();	
    
    return;
}



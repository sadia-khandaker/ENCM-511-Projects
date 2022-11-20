#include <xc.h>
#include "TimeDelay.h"


void delay_ms(uint16_t time_ms, uint8_t idle_on)
{
    //T2CON config
    T2CONbits.TSIDL = 0; //Continue timer operation in Idle mode
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0; // use internal clock
    // T2CONbits.TGATE = 0;
    
    //T2CONbits.TGATE = 1
    
    // Timer 2 interrupt config
    IPC1bits.T2IP = 2; //7 is highest and 1 is lowest priority
    IEC0bits.T2IE = 1; //enable timer interrupt

    
    
    IFS0bits.T2IF = 0; // Clear the Timer2 interrupt status flag
    
    PR2 = time_ms << 4; 
    
    TMR2 = 0;
    
    T2CONbits.TON = 1; // Start timer 2
    
    if(idle_on == 1)
    {
        Idle(); 
    }
    
    
    
    T2CONbits.TON=0; // Stop timer
    
    return;
}

//void delay_ms(uint16_t time_ms) {
//    //Configure TMR2
//    T2CONbits.TON = 0;      // disable TMR2
//    T2CONbits.TSIDL = 0;    // continue module operation in idle mode
//    T2CONbits.T32 = 0;      // use TMR2 as a 16-bit timer
//    T2CONbits.TCS = 0;      // use internal clock
//    TMR2 = 0;               // clear TMR2
//    
//    PR2 = time_ms * 16;
//    
//    // configure TMR2 interrupt
//    IPC1bits.T2IP = 0x07;   // set interrupt priority to 7, the highest
//    IFS0bits.T2IF = 0;      // clear TMR2 flag bit
//    IEC0bits.T2IE = 1;      // enable TMR2 interrupt
//    
//    T2CONbits.TON = 1;      //start TMR2 
//    
//    Idle();                 // put MCU in idle mode until TMR2 interrupt occurs
//    
//    return; 
//   
//   
//}

// Used in HW IO control Driver project 3
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
     IFS0bits.T2IF = 0; //Reset Timer2 interrupt flag and Return from ISR
    
    // TMR2flag = 1; // global variable created by user
    return;
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0; // Clear Timer2 interrupt flag
    if (sec == 0) {
        sec = 59;
        if (min == 0) {
            min = 59;
        } else {
            min--;
        }
    } else {
        sec--;
    }
}
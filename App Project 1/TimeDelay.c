#include <xc.h>
#include "TimeDelay.h"
#include "ChangeClk.h"


void delay_ms(uint16_t time_ms) {
    T2CONbits.TON = 0;      // disable TMR2
    T2CONbits.TSIDL = 0;    //operate in idle mode
    T2CONbits.T32 = 0;      // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0;      // use internal clock
    TMR2 = 0;               // clear TMR2
    PR2 = time_ms << 4;     // set PR2 to 16 * time_ms
    IPC1bits.T2IP = 7;      // set TMR2 interrupt priority to 7 (highest)
    IFS0bits.T2IF = 0;      // clear TMR2 flag bit
    IEC0bits.T2IE = 1;      // enable TMR2 interrupt
    T2CONbits.TON = 1;      //start TMR2 
    Idle();                 // idle mode
    return;
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
     IFS0bits.T2IF = 0; //Reset Timer2 interrupt flag and Return from ISR
     T2CONbits.TON=0; // stop timer
    return;
}

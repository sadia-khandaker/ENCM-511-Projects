#include "xc.h"
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"




// IOinit() without interrupts
/*
void IOinit(void)
{
    TRISBbits.TRISB8 = 0; // Make GPIO RB8 as a digital output
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output
    
    TRISAbits.TRISA4 = 1; // Makes GPIO RA4 as a digital input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    
    TRISBbits.TRISB4 = 1; // Makes GPIO RB4 as a digital input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RA4/CN0
    
    TRISAbits.TRISA2 = 1; // Makes GPIO RA2 as a digital input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA4/CN0
    
    return;
}
 */

// IOcheck() without timers)
/*

void IOcheck(void)
{
    uint32_t ctr = 0;
    
    while((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) //While only RA4 pb is pressed
        {
        ctr = 100000;
        while (ctr!=0) //approx 1 sec
        {
            ctr--;
        }
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; 
       }
    
    while((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) &&  (PORTAbits.RA2 == 1)) //While only RB4 pb is pressed
    {
        ctr = 250000;
        while (ctr!=0)  // approx 2 sec
        {
            ctr--;
        }
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; 
        
    }
    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //While only RA2 pb is pressed
    {
        ctr = 450000;
        while (ctr!=0)   //approx 3 sec
        {
            ctr--;
        }
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8
    }
    while(((PORTAbits.RA2 == 0) + (PORTAbits.RA4 == 0) + (PORTBbits.RB4 == 0)) > 1) //While 2 or more pbs are pressed
    {
        LATBbits.LATB8 = 1;
    }
    
    LATBbits.LATB8 = 0; // else led off
    
    return;
}
 * */

// IOCheck with Timers
/*
void IOcheck(void)
{
    
    while((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) //While only RA4 pb is pressed
        {
       delay_ms(1000, 1);
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; 
       }
    
    while((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) &&  (PORTAbits.RA2 == 1)) //While only RB4 pb is pressed
    {
        delay_ms(2000, 1);
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; 
        
    }
    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //While only RA2 pb is pressed
    {
        delay_ms(3000, 1);
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8
    }
    while(((PORTAbits.RA2 == 0) + (PORTAbits.RA4 == 0) + (PORTBbits.RB4 == 0)) > 1) //While 2 or more pbs are pressed
    {
        LATBbits.LATB8 = 1;
    }
    
    LATBbits.LATB8 = 0; // else led off
    
    return;
}
 */

// TO DO: Check if either of the IOinit() function work

//// IOinit() with IO interrupts (Provided by Dr. Vyas)

void IOinit(void) {
    AD1PCFG = 0xFFFF; // Set all analog pins to digital

    TRISBbits.TRISB8 = 0; // Make GPIO RB8 as a digital output
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output

    TRISAbits.TRISA4 = 1; // Makes GPIO RA4 as a digital input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1; // enable CN on CN0

    TRISBbits.TRISB4 = 1; // Makes GPIO RB4 as a digital input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN1IE = 1; // enable CN on CN1

    TRISAbits.TRISA2 = 1; // Makes GPIO RA2 as a digital input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN2bits.CN30IE = 1; // enable CN on CN30

    //CN Interrupt settings
    IPC4bits.CNIP = 6; //7 is highest priority, 1 is lowest, 0 is disabled interrupt
    IFS1bits.CNIF = 0; // Clear interrupt flag
    IEC1bits.CNIE = 1; //Enable CN interrupts

    return;
}

// Our IOinit function from Driver Project 3
//void IOinit() {
//
//    AD1PCFG = 0xFFFF; // Set all analog pins to digital
//
//    TRISBbits.TRISB8 = 0; // Set GPIO RB8 as digital output, which is connected to LED
//
//    TRISAbits.TRISA2 = 1; // Set GPIO RA2 as digital input, which is connected to PB1
//    TRISAbits.TRISA4 = 1; // Set GPIO RA4 as digital input, which is connected to PB2
//    TRISBbits.TRISB4 = 1; // Set GPIO RB4 as digital input, which is connected to PB3
//
//    CNPU1bits.CN0PUE = 1; // Enable pull-up resistor for RA4/CN0
//    CNPU2bits.CN30PUE = 1; // Enable pull-up resistor for RA2/CN30
//    CNPU1bits.CN1PUE = 1; // Enable pull-up resistor for RB4/CN1
//
//    CNEN1bits.CN0IE = 1; // Enable CN0 interrupt
//    CNEN2bits.CN30IE = 1; // Enable CN30 interrupt
//    CNEN1bits.CN1IE = 1; // Enable CN1 interrupt
//
//    IFS1bits.CNIF = 0; // Clear CN interrupt flag
//    IEC1bits.CNIE = 1; // Enable CN interrupt
//
//}

void IOcheck(void) {
    // While PB1 is pressed, LED blinks 0.5 seconds on and 0.5 seconds off. The message "PB1 is pressed" is displayed on the PC terminal.
    while ((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) //While only RA4 pb is pressed
    {
        delay_ms(500, 1); // 0.5 seconds on and 0.5 seconds off
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8
        Disp2String("PB1 is pressed\r\n"); // Display message on PC terminal
    }

    // While PB2 is pressed, LED blinks 2 seconds on and 2 seconds off. The message "PB2 is pressed" is displayed on the PC terminal.
    while ((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) && (PORTAbits.RA2 == 1)) //While only RB4 pb is pressed
    {
        delay_ms(2000, 1); // 2 seconds on and 2 seconds off
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8

        Disp2String("PB2 is pressed\r\n"); // Display message on PC terminal
    }

    // While PB3 is pressed, LED blinks 3 seconds on and 3 seconds off. The message "PB3 is pressed" is displayed on the PC terminal.
    while ((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //While only RA2 pb is pressed
    {
        delay_ms(3000, 1); // 3 seconds on and 3 seconds off
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8

        Disp2String("PB3 is pressed\r\n"); // Display message on PC terminal
    }

    // While 2 or more pbs are pressed, LED is on with no blinking. The message "PB___ and PB___ are pressed" is displayed on the PC terminal.
    while (((PORTAbits.RA2 == 0) + (PORTAbits.RA4 == 0) + (PORTBbits.RB4 == 0)) > 1) //While 2 or more pbs are pressed
    {
        LATBbits.LATB8 = 1; // Turns on LED connected to port RB8
        if ((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1)) // RA2 and RA4 pressed
        {

            Disp2String("PB3 and PB1 are pressed\r\n"); // Display message on PC terminal
        } else if ((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 0)) // RA2 and RB4 pressed
        {

            Disp2String("PB3 and PB2 are pressed\r\n"); // Display message on PC terminal
        } else if ((PORTAbits.RA2 == 1) && (PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 0)) // RA4 and RB4 pressed
        {

            Disp2String("PB1 and PB2 are pressed\r\n"); // Display message on PC terminal
        }
    }

    // While no pbs are pressed, LED is off. The message "Nothing pressed" is displayed on the PC terminal.
    while ((PORTAbits.RA2 == 1) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //While NO pbs are pressed
    {
        LATBbits.LATB8 = 0;
        Disp2String("No buttons are pressed\r\n"); // Display message on PC terminal
    }

}



// IO Check to demo HW debugger

//void IOcheck(void)
//{
//    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
//    delay_ms(400,1);   // 400 msec delay to filter out debounces 
//    IEC1bits.CNIE = 1; //Enable CN interrupts to detect pb release
//    
//    while((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) //While only RA4 pb is pressed
//    {
//        LATBbits.LATB8 = 1; 
//    }
//    
//    while((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) &&  (PORTAbits.RA2 == 1)) //While only RB4 pb is pressed
//    {
//        LATBbits.LATB8 = 1; 
//    }
//    
//    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //While only RA2 pb is pressed
//    {
//        LATBbits.LATB8 = 1; // Turns on LED connected to port RB8
//    }
//    
//    return;
//
//}


///// Change of pin Interrupt subroutine

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    CNflag = 1; // global user defined flag - use only if needed
    IFS1bits.CNIF = 0; // clear IF flag
    Nop();

    return;
}

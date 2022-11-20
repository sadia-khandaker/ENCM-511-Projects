

#include <xc.h>
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"
#include <time.h>
#include <string.h>




uint16_t  min;
uint16_t sec;
uint16_t counter; 
uint16_t flag;



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



//// IOinit() with IO interrupts
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
    IFS1bits.CNIF = 0; // Clear interrupt flag
    IEC1bits.CNIE = 1; //Enable CN interrupts
    
    return;
}





//void IOcheck(void){
//     while (PORTBbits.RB4 == 0) {
//        delay_ms(500, 1); // 1 sec delay
//        LATBbits.LATB8 = LATBbits.LATB8 ^ 1;
//        NewClk(8); // speed up clock just for display
//        Disp2String("\r RB4 is pressed       ");
//        NewClk(32); // slow down clock for delay and other tasks
//        min++;
//        if (min > 59) {
//            min = 0;
//        }
//        Disp2String("\r");
//        Disp2String(itoaconv(min));
//        Disp2String("m : ");
//        Disp2String(itoaconv(sec));
//        Disp2String("s");
//    }
//}

// Returns a pointer to a string of the double digit representation of the given value
//char * double_digits(int value) {
//    static char buffer[3];
//    sprintf = sprintf(buffer, "%02d", value);
//    return buffer;
//   
//}

void format_time(void){
    static char buffer [12];
    sprintf(buffer, "\r%02d m : %02ds",  min, sec);
    Disp2String(buffer);
}



void IOCheck(void){
    while ((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) {
        // sets the minute to be counted down from
        if (PORTAbits.RA2 == 0) {
            // When PB1 is pressed, the minute count increments by 1 minute from 0 to a maximum of 59.
            if (min < 59) {
                min++;
            } else {
                min = 0;
            }
            // The incrementing minute count should be displayed on the PC terminal as below.
            format_time();
            delay_ms(500, 1);
        }
        // Clears any ALARM message from previous countdowns
        if (PORTAbits.RA2 == 0) {
            Disp2String("\r");
        }
    }
    while ((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1)) {
        if (PORTBbits.RB4 == 0) {
            // When PB2 is pressed, the seconds count increments by 1 sec from 0 to a maximum of 59.
            if (sec < 59) {
                sec++;
            } else {
                sec = 0;
            }
            // The incrementing second count should be displayed on the PC terminal as below.
            format_time();
            XmitUART2(' ', 9);
            delay_ms(500, 1);
        }
        // Clears any ALARM message from previous countdowns
        if (PORTAbits.RA2 == 0) {
            Disp2String("\r");
        }

        }


while((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) {
    counter++;
    if (counter < 30) {
        flag = (!flag);
    }
    delay_ms(3000, 1);
    // if long press, reset the timer
    if (counter > 30) {
        //Disp2String("PB3 is pressed\r\n"); // Display message on PC terminal
        //delay_ms(3000, 1); // 3 seconds on and 3 seconds off
        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8
        flag = 0;
        min = 0;
        sec = 0;
        format_time();
    }
    

}
//void IOcheck(void){
//    while ((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) //While only RA4 pb is pressed
//    {
//        min ++;
//        format_time();
//        XmitUART2(' ', 9);
//       
//        delay_ms(500,1); 
//        
//     
//        
//    }
//    
//     while ((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) && (PORTAbits.RA2 == 1)) //While only RB4 pb is pressed
//    {
//         sec++; 
//         format_time();
//        XmitUART2(' ', 9);
//        delay_ms(500, 1);
//         
//        
//    }
//    while(PORTAbits.RA2 == 1 && PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1) // while RB4, pb3 is pressed
//    {
//        counter++;
//        delay_ms(100,1);  // delay of 100 ms  
//    }
//    
//    
//    while ((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //While only RA2 pb is pressed
//    {
//        counter++;
//        
//        if (counter =! 0 && counter < 30) {
//            flag = (!flag);
//        }
//        if (counter > 30) {
//            
//            Disp2String("PB3 is pressed\r\n"); // Display message on PC terminal
//            //delay_ms(3000, 1); // 3 seconds on and 3 seconds off
//            LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8
//            flag = 0;
//            min = 0;
//            sec = 0;
////            sprintf (str, "%dm : %ds", min, sec);
////            Disp2String(str);
//            format_time();
//            
//        }
//        // if sec<3, either start or pause the timer
//        else {
//            Disp2String("PB3 is pressed\r\n"); // Display message on PC terminal
//            delay_ms(3000, 1); // 3 seconds on and 3 seconds off
//            LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8
//            LATBbits.LATB8 = 0;
//            
//            if (sec == 0) {
//                sec = 1;
//            } else {
//                sec = 0;
//            }
//        }
//        
//   
//      
//    }
//    
//    
//
//    
//    
//    
//}


// CN interrupt routine

//void IOcheck(void)
//{
//    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
//    delay_ms(400,1);   // 400 msec delay to filter out debounces 
//    IEC1bits.CNIE = 1; //Enable CN interrupts to detect pb release
//    
//    while((PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1) && (PORTAbits.RA2 == 1)) //While only RA4 pb is pressed
//       {
//        delay_ms(500,1);   // 1 sec delay
//        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; 
//        NewClk(8); // speed up clock just for display
//        Disp2String("\r RA4 is pressed       ");
//        NewClk(32); // slow down clock for delay and other tasks
//       }
//    
//    while((PORTBbits.RB4 == 0) && (PORTAbits.RA4 == 1) &&  (PORTAbits.RA2 == 1)) //While only RB4 pb is pressed
//    {
//        delay_ms(2000,1);   // 1 sec delay
//        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; 
//        NewClk(8);
//        Disp2String("\r RB4 is pressed       ");
//        NewClk(32);
//    }
//    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //While only RA2 pb is pressed
//    {
//        delay_ms(3000,1);   // 1 sec delay
//        LATBbits.LATB8 = LATBbits.LATB8 ^ 1; // Turns on LED connected to port RB8
//        NewClk(8);
//        Disp2String("\r RA2 is pressed       ");
//        NewClk(32);
//    }
//    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 1)) //While RA2 and RA4 pb is pressed
//    {
//        LATBbits.LATB8 = 1; // Turns on LED connected to port RB8
//        NewClk(8);
//        Disp2String("\r RA2 & RA4 are pressed    ");
//        NewClk(32);
//    }
//    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 0)) //While RA2 and RB4 pb is pressed
//    {
//        LATBbits.LATB8 = 1; // Turns on LED connected to port RB8
//        NewClk(8);
//        Disp2String("\r RA2 & RB4 are pressed    ");
//        NewClk(32);
//    }
//    while((PORTAbits.RA2 == 1) && (PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 0)) //While only RB4 and RA4 pb is pressed
//    {
//        LATBbits.LATB8 = 1; // Turns on LED connected to port RB8
//        NewClk(8);
//        Disp2String("\r RA4 & RB4 are pressed    ");
//        NewClk(32);
//    }
//    while((PORTAbits.RA2 == 0) && (PORTAbits.RA4 == 0) && (PORTBbits.RB4 == 0)) //While all pb is pressed
//    {
//        LATBbits.LATB8 = 1; // Turns on LED connected to port RB8
//        NewClk(8);
//        Disp2String("\r All are pressed         ");
//        NewClk(32);
//    }
//    if((PORTAbits.RA2 == 1) && (PORTAbits.RA4 == 1) && (PORTBbits.RB4 == 1)) //No pbs are pressed
//    {
//        LATBbits.LATB8 = 0; // Turns on LED connected to port RB8
//        NewClk(8);
//        Disp2String("\r Nothing pressed      ");
//        NewClk(32);
//        T2CONbits.TON = 0; // Disable timer
//        IEC0bits.T2IE = 0; //Disable timer interrupt
//    }
//    
//    return;
//
//}
//






///// Change of pin Interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) __CNInterrupt(void)
{
	CNflag = 1;  // global user defined flag - use only if needed
	IFS1bits.CNIF = 0;		// clear IF flag
    T2CONbits.TON = 0; // Disable timer
    IEC0bits.T2IE = 0; //Disable timer interrupt
    IOcheck();
	Nop();	
    
    return;
}

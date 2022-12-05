/*
 * File:   ADC.c
 * Author: Ethan Kerr
 *
 * Created on November 17, 2022, 4:20 AM
 */


#include <xc.h>
#include "UART2.h"
#include "ADC.h"
#include "ChangeClk.h"


uint16_t do_ADC(void) {
    uint16_t ADCvalue;
    
    //ADC initialization
    AD1CON1bits.ADON = 1;
    AD1CON1bits.FORM = 0b00;        //integer data output format
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.ASAM = 0;
    
    AD1CON2bits.VCFG = 0b000;       //Voltage ref config bits (AVdd, AVss)
    AD1CON2bits.CSCNA = 0;          //Do not scan inputs
    AD1CON2bits.BUFM = 0;           //Buffer configured as one 16-word buffer
    AD1CON2bits.ALTS = 0;           //Always uses MUXA input multiplexer settings
    
    AD1CON3bits.ADRC = 0;           //use system clock
    AD1CON3bits.SAMC = 0b11111;    //Slowest sampling time = 31*2/fclk
    
    AD1CHSbits.CH0NA = 0;           //Channel 0 neg input is VR-
    AD1CHSbits.CH0SA = 0b0101;      //Channel 0 pos input is AN5 (RA3)
    AD1PCFGbits.PCFG5 = 0;          //AN5 pin configured in analog, I/O port read disabled, A/D samples pin voltage
    
    
    
    //ADC Sampling and Conversion
    AD1CON1bits.SAMP = 1;
    while(AD1CON1bits.DONE == 0) {} // wait
    ADCvalue = ADC1BUF0;
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.ADON = 0;
    return (ADCvalue);
}

// Does ADC on AN5 and dislays ADCBUF0 value and proportional number of markers on Terminal
void DispADC(void)
{
        NewClk(8);
        uint16_t adcbuf; // variable to store ADCBUF0 value
        uint8_t MarkerCnt; // Stores number of markers needed
        uint8_t MarkerCntOld; 
        
        adcbuf = do_ADC(); // Does ADC conversion on AN5/IO8
        MarkerCnt = adcbuf/20 + 1; //Calculates number of markers
        
        if (MarkerCntOld != MarkerCnt) // refresh display only if ADC value has changed
        {
            XmitUART2('\r', 1);
            XmitUART2(' ', MarkerCntOld + 15); // Clears line
            XmitUART2('\r', 1);
            XmitUART2('*', MarkerCnt); //Displays * MarkerCnt times
            XmitUART2(' ', 1);
            Disp2Hex(adcbuf); // Displays ADCBUF0 value at end of markers
            MarkerCntOld = MarkerCnt;
        }
         
        NewClk(32);
        
    
    return;
}


void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void)
{

    IFS0bits.AD1IF = 0;			// Clear the ADC1 Interrupt Flag

}



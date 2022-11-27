/*
 * File:   ADC.c
 * Author: Ethan Kerr
 *
 * Created on November 17, 2022, 4:20 AM
 */


#include <xc.h>
#include "UART2.h"

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
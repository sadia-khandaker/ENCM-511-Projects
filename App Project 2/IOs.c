/*
 * File:   IOs.c
 * Author: Sadia Khandaker
 *
 * Created on December 4, 2022
 */


#include "xc.h"
#include "ChangeClk.h"
#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"
#include "ADC.h"




//// IOinit() with IO interrupts
// Sets outputs and CN interrupts on 3 PBs; Output for LED 
void IOinit(void)
{
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    TRISBbits.TRISB8 = 0; // Make GPIO RB8 as a digital output
    LATBbits.LATB8 = 0; // Make GPIO RB8 as a digital output
    
    TRISAbits.TRISA4 = 1; // Makes GPIO RA4 as a digital input
    CNPU1bits.CN0PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN0IE = 1;  // enable CN on CN0
    
    TRISBbits.TRISB4 = 1; // Makes GPIO RB4 as a digital input
    CNPU1bits.CN1PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN1bits.CN1IE = 1;  // enable CN on CN1
    
    TRISAbits.TRISA2 = 1; // Makes GPIO RA2 as a digital input
    CNPU2bits.CN30PUE = 1; // Enables pull up resistor on RA4/CN0
    CNEN2bits.CN30IE = 1;  // enable CN on CN30
    
    TRISBbits.TRISB0 = 0; //Sets RB0-GPIO4 as output (LED)
    TRISBbits.TRISB1 = 0; //Sets RB1-GPIO5 as output (LED)
    TRISBbits.TRISB2 = 0; //Sets RB0-GPIO4 as output (LED)

    LATBbits.LATB0 = 0; // Turns on LED connected to port RB0
    LATBbits.LATB1 = 0; // Turns on LED connected to port RB1
    LATBbits.LATB2 = 0;  // Turns on LED connected to port RB2

    
    //CN Interrupt settings
    IPC4bits.CNIP = 6; //7 is highest priority, 1 is lowest, 0 is disabled interrupt
    IFS1bits.CNIF = 0; // Clear interrupt flag
    IEC1bits.CNIE = 1; //Enable CN interrupts
    
    return;
}


// CN interrupt routine = Used in Driver project 4

//void IOcheck(void)
//{
//    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
//    delay_ms(400,);   // 400 msec delay to filter out debounces 
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

uint8_t pb_flag = 0; // Flag to indicate that a push button has been pressed



/**
 * Determines if the any of the push buttons are pressed
 * @return returns 1 if any of the 3 push buttons are pressed, 0 otherwise
 */
uint8_t pb_pressed(void) {
    // Check if any of the push buttons are pressed
    if ((PORTAbits.RA4 == 0) || (PORTBbits.RB4 == 0) || (PORTAbits.RA2 == 0)) {
        return 1; // Return 1 if any of the three buttons has been pressed
    } else { 
        return 0;// Otherwise, return 0, if it hasn't
    }
}



/**
 * Helper function that toggles LED1
 */
void toggle_LED1(void) {
    LATBbits.LATB0 = ~LATBbits.LATB0; // Toggle LED connected to port RB0, if it was on, turn it off, if it was off, turn it on
}

/**
 * Helper function that toggles LED2
 */
void toggle_LED2(void) {
    LATBbits.LATB1 = ~LATBbits.LATB1; // Toggle LED connected to port RB1, if it was on, turn it off, if it was off, turn it on
}

/**
 * Helper function that toggles LED3
 */
void toggle_LED3(void) {
    LATBbits.LATB2 = ~LATBbits.LATB2; // Toggle LED connected to port RB2, if it was on, turn it off, if it was off, turn it on
}


/**
 * Helper function that toggles all of the three LEDs
 */
void toggle_LEDs(void) {
    toggle_LED1();
    toggle_LED2();
    toggle_LED3();
}


/**
 * Helper function that generates a random sequence of 0s and 1s for each round 
 * @param round - the current round user is on
 * @param round_array  - the array that stores the random sequence of 0s and 1s for the current round
 */
void generate_sequence(int round, uint8_t *round_array) {
    int i;
    srand(0x3300);
//    uint8_t bit;
    for (i = 0; i < round; i++) {
//        bit = rand() % 2;
        round_array[i] = rand() % 2;
    }
    return;
}

/**
 * Sets the speed of the game based on the potentiometer
 * @return - time interval between each LED turning on 
 */
uint16_t set_interval() {
    uint16_t time_interval = 1500; // return this if adc has not been used to set interval
    uint16_t adc_value = do_ADC(); // get adc value from potentiometer
    
    // if potentiometer is not used, return default time interval
    if (adc_value == 0) {
        return time_interval;
    }
    
    // if potentiometer is used, return time interval based on adc value
    if (adc_value <= 170.5) {
            time_interval = 500; // 500 ms
        } else if (adc_value > 170.5 && adc_value <= 341) {
            time_interval = 1000; // 1 second
        } else if (adc_value > 341 && adc_value <= 511.5) {
            time_interval = 1500; // 1.5 seconds
        } else if (adc_value > 511.5 && adc_value <= 682) {
            time_interval = 2000; // 2 seconds
        } else if (adc_value > 682 && adc_value <= 852.5) {
            time_interval = 2500; // 2.5 seconds
        } else if (adc_value > 852.5 && adc_value <= 1023) {
            time_interval = 3000; // 3 seconds
        }
    // return time interval
    return time_interval;
}










/**
 * Light up LED lights in a sequence
 * @param time_ms time in milliseconds to delay between each light up
 * @param round_array array of different number of elements that hold the the bits for the LED lights
 * @param size size of the round_array
 */
void LED_sequence(uint16_t time_ms, const uint8_t round_array[], int size) {
    int i;
    uint8_t led_array[3];
    for (i = 0; i < size; i++) {
        led_array[ i % 3 ] = round_array[ i ];
        if (led_array[ 0 ] == 1) {
            // turn on led
            toggle_LED1();
            // delay
            delay_ms(time_ms);
            // turn off led
            toggle_LED1();
            delay_ms(time_ms);
        } else {
            delay_ms(time_ms);
        }
        if (led_array[ 1 ] == 1) {
            // light up led
            toggle_LED2();
            delay_ms(time_ms);
            toggle_LED2();
            // turn off led
            delay_ms(time_ms);
        } else {
            delay_ms(time_ms);
        }
        if (led_array[ 2 ] == 1) {
            // light up led
            toggle_LED3();
            delay_ms(time_ms);
            // turn off led
            toggle_LED3();
            delay_ms(time_ms);
        } else {
            delay_ms(time_ms);
        }

    }

}


/**
 * Check if the push button 1 has been pressed
 * @return 0 if pb1 has not been pressed and 1 if pb1 has been pressed
 */
uint8_t pb1_pressed() {
    if (PORTAbits.RA2 == 0) {
        return 1;
    } else {
        return 0;
    }
}


/**
 * Check if the push button 2 has been pressed
 * @return 1 if the push button 1 has been pressed, 0 otherwise
 */
uint8_t pb2_pressed() {
    if (PORTBbits.RB4 == 0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Check if the push button 3 has been pressed
 * @return 1 if the push button 3 has been pressed, 0 otherwise
 */
uint8_t pb3_pressed() {
    if (PORTAbits.RA4 == 0) {
        return 1;
    } else {
        return 0;
    }
}





/**
 * Helper function that starts the game
 */
uint8_t game_started = 0;
void start_game() {
    while (PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1 && game_started==0 ) {
        game_started = 1;
        NewClk(8);
        Disp2String("\r Game started!      ");
        NewClk(32);
        T2CONbits.TON = 1; // Enable timer
        IEC0bits.T2IE = 1; //Enable timer interrupt
        
    }
}




/**
 * If the second button is pressed twice, reset the game
 */
uint16_t button_pressed = 0;
void stop_game() {
    while (PORTAbits.RA2 == 1 && PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1 && game_started==1 && button_pressed<=2) {
        button_pressed++;
        // if the second button is pressed twice, reset the game
        if (button_pressed == 2) {
            game_started = 0;
            button_pressed = 0;

        }

    }
}

// first round. 3 led lights, store either 1 or 0, for each led, in an array
uint8_t round1[3] = {0, 0, 0};
// second round. 4 led lights, store either 1 or 0, for each led, in an array
uint8_t round2[4] = {0, 0, 0, 0};
// third round 5. led lights, store either 1 or 0, for each led, in an array
uint8_t round3[5] = {0, 0, 0, 0, 0};
// fourth round 6. led lights, store either 1 or 0, for each led, in an array
uint8_t round4[6] = {0, 0, 0, 0, 0, 0};
// fifth round 7. led lights, store either 1 or 0, for each led, in an array
uint8_t round5[7] = {0, 0, 0, 0, 0, 0, 0};
// sixth round 8. led lights, store either 1 or 0, for each led, in an array
uint8_t round6[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// seventh round 9. led lights, store either 1 or 0, for each led, in an array
uint8_t round7[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
// eighth round 10. led lights, store either 1 or 0, for each led, in an array
uint8_t round8[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// ninth round 11. led lights, store either 1 or 0, for each led, in an array
uint8_t round9[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// tenth round 12. led lights, store either 1 or 0, for each led, in an array
uint8_t round10[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * Play the game
 */
int round = 1;
void play_game(){
    while (game_started == 1) {
        // default time interval is 1.5 seconds so 1500 milliseconds
        uint16_t time_interval = set_interval();
        // generate random sequence of 0s and 1s for each round
        if (round == 1) {
            generate_sequence(round, round1);
            // now keep track of what buttons have been pressed, and if it matches the sequence, then continue to the next round
            LED_sequence(time_interval, round1, round);
            round++;
        } else if (round == 2) {
            generate_sequence(round, round2);
            LED_sequence(time_interval, round2, round);
            round++;
        } else if (round == 3) {
            generate_sequence(round, round3);
            LED_sequence(time_interval, round3, round);
            round++;
        } else if (round == 4) {
            generate_sequence(round, round4);
            LED_sequence(time_interval, round4, round);
            round++;
        } else if (round == 5) {
            generate_sequence(round, round5);
            LED_sequence(time_interval, round5, round);
            round++;
        } else if (round == 6) {
            generate_sequence(round, round6);
            LED_sequence(time_interval, round6, round);
            round++;
        } else if (round == 7) {
            generate_sequence(round, round7);
            LED_sequence(time_interval, round7, round);
            round++;
        } else if (round == 8) {
            generate_sequence(round, round8);
            LED_sequence(time_interval, round8, round);
            round++;
        } else if (round == 9) {
            generate_sequence(round, round9);
            LED_sequence(time_interval, round9, round);
            round++;
        } else if (round == 10) {
            generate_sequence(round, round10);
            LED_sequence(time_interval, round10, round);
            round++;
        }

    }

    // Game over
//    char score[10];
//    Disp2String("\r Game Over! \r Your score is: ");
//    sprintf(score, "%d", round);
//    Disp2String(score);
//    Disp2String("\r");
//    // reset the game
    game_started = 0;
    round = 1;
}


/**
 * Record the sequence of which the user presses the buttons, checks if the sequence is correct
 * @param round_array  array of different number of elements that hold the the bits for the LED lights
 * @param size size of the round_array
 */
void record_sequence(uint8_t *round_array, int size) {
    int i;
    int pressed = 0;
    // hold temp array to compare with round_array
    uint8_t temp_array[size];
    while (pressed < size && game_started == 1) {
        if (temp_array[ pressed ] != round_array[ pressed ]) {
            game_started = 0;
        }

        if (pb1_pressed()) {
            // store 1 in temp_array
            temp_array[ pressed ] = 1;
            // check

            pressed++;
            // wait for pb1 to be released
            while (pb1_pressed()) {
                // do nothing
            }
        }
        // check if pb2 has been pressed
        if (pb2_pressed()) {

            // store 0 in temp_array
            temp_array[ pressed ] = 1;
            // increment pressed
            pressed++;
            // wait for pb2 to be released
            while (pb2_pressed()) {

            }
        }
        if (pb3_pressed()) {

            // store 0 in temp_array
            temp_array[ pressed ] = 0;
            // increment pressed
            pressed++;
            // wait for pb2 to be released
            while (pb3_pressed()) {

            }
        }

    }


}



void play(void) {
    start_game();
    play_game();
    stop_game();
}



///// Change of pin Interrupt subroutine 

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
	IFS1bits.CNIF = 0;		// clear IF flag
	Nop();	
    return;
}
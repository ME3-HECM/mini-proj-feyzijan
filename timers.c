#include <xc.h>
#include "timers.h"
extern unsigned char test_mode;

// Always write High-Reg first as it updates when Low-reg is written
/************************************
 * Function to set up timer 0
************************************/
void Timer0_init() //unsigned char test_mode
{
    if(test_mode ){
    //LATDbits.LATD7 =1; //for debugging
    T0CON1bits.T0CS=0b010; // Fosc/4 = 16Mhz
    T0CON1bits.T0ASYNC=1; // needed to ensure correct operation with Fosc/4 as clock source
    T0CON1bits.T0CKPS=0b1000 ; // Pre-scaler = 1:256: for one second we need 244 
    TMR0H = 0; 
    TMR0L = 0;
    } else {
    T0CON1bits.T0CS=0b100; //RealMode: LFINTOSC  31000 Hz
    T0CON1bits.T0CKPS=0b1011 ; // Prescale = 1:2048
    //NOTE: START FROM 0b0010101100100011 to overflow in one hour 
    TMR0H = 0b00101011;
    TMR0L = 0b00100011;
    }
   
    T0CON0bits.T016BIT=1;	//16bit mode	
    
    PIE0bits.TMR0IE = 1; //enable interrupt
    IPR0bits.TMR0IP = 1; //priority high
    
    
    T0CON0bits.T0EN=1;	//start the timer
}

/************************************
/ Function to initialise Timer1 to follow the overflow count of Timer0 
 * and thereby represent hour of the day
************************************/
void Timer1_init(void) // see page 365 for possible error cause
{ 
    TMR1CLKbits.CS = 0b1000; // select clock source: TMR0 overflow
    T1CONbits.CKPS = 0b00; // select pre-scaler: 1:1 
    PIE5bits.TMR1IE = 1; //enables interrupt
    IPR5bits.TMR1IP = 1; //interrupt priority set high
    /* Initialise the timer registers at 0 */
    TMR1H = 0;
    TMR1L = 0;
    /*Enable timer and counter */
    T1CONbits.ON = 1; 
    T1GCONbits.GE = 1;
}
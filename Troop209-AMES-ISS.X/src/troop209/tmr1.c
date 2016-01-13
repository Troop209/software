#include "../drivers/system.h"
#include "tmr1.h" 

/* Initializes the timer */
static void startTimer1(unsigned short prescale, unsigned period)
{
    T1CONbits.TON = 0;           // Turn off the the timer
    T1CONbits.TCKPS = prescale;  // Set prescale (0-3)
    T1CONbits.TCS = 0b00;        // Clock source is (f_osc / 4)
    TMR1 = 0; 
    PR1 = period;
    T1CONbits.TON = 1;           // Turn on the the timer
} 

/* Stop Timer1 */
static void stopTimer1()
{
    T1CONbits.TON = 0;

} // end of setPWMDutyCycle

const TIMER1 timer1 = {start:startTimer1, stop:stopTimer1};

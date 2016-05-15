#include "../core/system.h"
#include "tmr5.h" 

/* Initializes the timer */
static void startTimer5(unsigned short prescale, unsigned period)
{
    T5CONbits.TON = 0;           // Turn off the the timer
    T5CONbits.TCKPS = prescale;  // Set prescale (0-3)
    T5CONbits.TCS = 0b00;        // Clock source is (f_osc / 4)
    TMR5 = 0; 
    PR5 = period;
    T5CONbits.TON = 1;           // Turn on the the timer
} 

/* Stop Timer1 */
static void stopTimer5()
{
    T5CONbits.TON = 0;

} // end of setPWMDutyCycle

const TIMER5 timer5 = {start:startTimer5, stop:stopTimer5};

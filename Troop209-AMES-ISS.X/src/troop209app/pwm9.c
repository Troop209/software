#include "../core/system.h"
#include "pwm9.h"
#include "tmr5.h"

/* Initializes the OC module */
static void initRawPwm9(unsigned short clock)
{
    /* disable module before configuring */
    OC9CON1 &= 0xFFF8;

    /* OC config */
    OC9RS = 0;   /* set the PWM period to a frequency of 5kHz */
    OC9R  = 0;              /* set the Duty Cycle to 0% */
    OC9CON2 = 0x001F;       /* set PWM 1 to sync with itself, non inverted signal (active high) */
    OC9CON1 = 0x0008;       /* Trigger mode 1, Mode 0 (disable  PWM) */
    OC9CON1bits.OCTSEL = clock;  /* Set clock source */

    /* configure pins */
    _TRISF3 = 0;            /* PWM 1 is an output */
    _LATF3 = 0;             /* drive output low */

    __builtin_write_OSCCONL(OSCCON & 0xBF);     /* unlock Peripheral Pin Select Registers */
    _RP16R = 35;
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock Peripheral Pin Select Registers */

} 

/* Sets the period and duty cycle of PWM 1 */
static void turnOnRawPwm9(unsigned period, unsigned dutycycle)
{
    /* now set the duty cycle register to the new duty cycle value scaled to the current frequency */
    OC9RS = period;
    OC9R = dutycycle;  // round(((unsigned long)(period) * dutycycle) / 100.00);
    OC9CON1 |= 0x0006;

} 

/* Turns off PWM 2 */
static void turnOffRawPwm9(unsigned period, unsigned dutycycle)
{
    OC9CON1 |= 0xFFF8;

} 

const PWMA pwma9 = {turnOn:turnOnRawPwm9, turnOff:turnOffRawPwm9, init:initRawPwm9};

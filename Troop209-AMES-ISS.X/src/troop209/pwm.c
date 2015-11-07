#include "../drivers/system.h"
#include "pwm.h"

/* Initializes the OC module */
static void initRawPwm1(void)
{
    /* disable module before configuring */
    OC1CON1 &= 0xFFF8;

    /* OC config */
    OC1RS = 0;   /* set the PWM period to a frequency of 5kHz */
    OC1R  = 0;              /* set the Duty Cycle to 0% */
    OC1CON2 = 0x001F;       /* set PWM 1 to sync with itself, non inverted signal (active high) */
    OC1CON1 = 0x1C08;       /* Clock source Fcyc, trigger mode 1, Mode 0 (disable  PWM) */

    /* configure pins */
    _PCFG8 = 1;             /* PWM 2 is digital */
    _TRISB8 = 0;            /* PWM 2 is an output */
    _LATB8 = 0;             /* drive output low */

    __builtin_write_OSCCONL(OSCCON & 0xBF);     /* unlock Peripheral Pin Select Registers */
    _RP8R = 17 + 1;                             /* PWM offset + channel */
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock Peripheral Pin Select Registers */

} // end of initPWM

/* Sets the duty cycle of PWM 2 to that of the passed value (as an integer percent) */
static void turnOnRawPwm1(unsigned period, unsigned dutycycle)
{
    /* now set the duty cycle register to the new duty cycle value scaled to the current frequency */
    OC1RS = period;
    OC1R = round(((unsigned long)(period) * dutycycle) / 100.00);
    OC1CON1 |= 0x0006;

} // end of setPWMDutyCycle

/* Sets the duty cycle of PWM 2 to that of the passed value (as an integer percent) */
static void turnOffRawPwm1(unsigned period, unsigned dutycycle)
{
    OC1CON1 |= 0xFFF8;

} // end of setPWMDutyCycle

const PWM pwm1 = {turnOn:turnOnRawPwm1, turnOff:turnOffRawPwm1, init:initRawPwm1};
//    PWM pwm2 = {turnOn:turnOnPwm2, init:initPwm2};

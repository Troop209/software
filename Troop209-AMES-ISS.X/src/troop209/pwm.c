#include "../drivers/system.h"
#include "pwm.h"

/* Initializes the OC module */
static void initRawPwm1(unsigned short clock)
{
    /* disable module before configuring */
    OC1CON1 &= 0xFFF8;

    /* OC config */
    OC1RS = 0;   /* set the PWM period to a frequency of 5kHz */
    OC1R  = 0;              /* set the Duty Cycle to 0% */
    OC1CON2 = 0x001F;       /* set PWM 1 to sync with itself, non inverted signal (active high) */
    OC1CON1 = 0x0008;       /* Trigger mode 1, Mode 0 (disable  PWM) */
    OC1CON1bits.OCTSEL = clock;  /* Set clock source */

    /* configure pins */
    _PCFG8 = 1;             /* PWM 1 is digital */
    _TRISB8 = 0;            /* PWM 1 is an output */
    _LATB8 = 0;             /* drive output low */

    __builtin_write_OSCCONL(OSCCON & 0xBF);     /* unlock Peripheral Pin Select Registers */
    _RP8R = 17 + 1;                             /* PWM offset + channel */
    _RP16R = 0;                             /* PWM offset + channel */
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock Peripheral Pin Select Registers */

} 

/* Sets the period and duty cycle of PWM 1 */
static void turnOnRawPwm1(unsigned period, unsigned dutycycle)
{
    /* now set the duty cycle register to the new duty cycle value scaled to the current frequency */
    OC1RS = period;
    OC1R = round(((unsigned long)(period) * dutycycle) / 100.00);
    OC1CON1 |= 0x0006;

} 

/* Turns off PWM 2 */
static void turnOffRawPwm1(unsigned period, unsigned dutycycle)
{
    OC1CON1 |= 0xFFF8;

} 

/* Initializes the OC module */
static void initRawPwm2(unsigned short clock)
{
    /* disable module before configuring */
    OC2CON1 &= 0xFFF8;

    /* OC config */
    OC2RS = 0;   /* set the PWM period to a frequency of 5kHz */
    OC2R  = 0;              /* set the Duty Cycle to 0% */
    OC2CON2 = 0x001F;       /* set PWM 2 to sync with itself, non inverted signal (active high) */
    OC2CON1 = 0x0008;       /* Trigger mode 1, Mode 0 (disable  PWM) */
    OC2CON1bits.OCTSEL = clock;  /* Set clock source */

    /* configure pins */
    _PCFG9 = 1;             /* PWM 2 is digital */
    _TRISB9 = 0;            /* PWM 2 is an output */
    _LATB9 = 0;             /* drive output low */

    __builtin_write_OSCCONL(OSCCON & 0xBF);     /* unlock Peripheral Pin Select Registers */
    _RP9R = 17 + 2;                             /* PWM offset + channel */
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock Peripheral Pin Select Registers */

} 

/* Sets the duty cycle of PWM 2 to that of the passed value (as an integer percent) */
static void turnOnRawPwm2(unsigned period, unsigned dutycycle)
{
    /* now set the duty cycle register to the new duty cycle value scaled to the current frequency */
    OC2RS = period;
    OC2R = round(((unsigned long)(period) * dutycycle) / 100.00);
    OC2CON1 |= 0x0006;

} 

/* Sets the duty cycle of PWM 2 to that of the passed value (as an integer percent) */
static void turnOffRawPwm2(unsigned period, unsigned dutycycle)
{
    OC2CON1 |= 0xFFF8;

} 

const PWM pwm1 = {turnOn:turnOnRawPwm1, turnOff:turnOffRawPwm1, init:initRawPwm1},
          pwm2 = {turnOn:turnOnRawPwm2, turnOff:turnOffRawPwm2, init:initRawPwm2};

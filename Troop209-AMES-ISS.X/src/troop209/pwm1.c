#include "../drivers/system.h"
#include "pwm1.h"
#include "tmr1.h"

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
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock Peripheral Pin Select Registers */

} 

/* Initializes the OC module */
static void initWithTimer1(unsigned period, unsigned cycle)
{
    /* disable module before configuring */
    OC1CON1 &= 0xFFF8;

    /* OC config */
    OC1RS = period;   
    OC1R  = cycle;              /* set the Duty Cycle to 0% */
    OC1CON2 = 0x001F;       /* set PWM 1 to sync with itself, non inverted signal (active high) */
    OC1CON1 = 0x0008;       /* Trigger mode 1, Mode 0 (disable  PWM) */
    OC1CON1bits.OCTSEL = PWM_TIMER_SELECTION_TIMER1;  /* Set clock source */

    /* configure pins */
    _PCFG8 = 1;             /* PWM 1 is digital */
    _TRISB8 = 0;            /* PWM 1 is an output */
    _LATB8 = 0;             /* drive output low */

    __builtin_write_OSCCONL(OSCCON & 0xBF);     /* unlock Peripheral Pin Select Registers */
    _RP8R = 17 + 1;                             /* PWM offset + channel */
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock Peripheral Pin Select Registers */

} 

static void turnOnPwm1() {
    OC1CON1 |= 0x0006;
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

const PWM pwm1 = {turnOn:turnOnRawPwm1, turnOff:turnOffRawPwm1, init:initRawPwm1, initTimer1: initWithTimer1, turnOnPwm1: turnOnPwm1};

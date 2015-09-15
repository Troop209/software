#include "system.h"
#include "led.h"

#define MIN_DUTYCYCLE   (0)
#define MAX_DUTYCYCLE   (100)

/* Initializes the OC module */
static void initPwm1(void)
{
    /* disable module before configuring */
    OC1CON1 &= 0xfff8;

    /* OC config */
    OC1RS = FCY/5000 - 1;   /* set the PWM period to a frequency of 5kHz */
    OC1R  = 0;              /* set the Duty Cycle to 0% */
    OC1CON2 = 0x001F;       /* set PWM 1 to sync with itself, non inverted signal (active high) */
    OC1CON1 = 0x1C08;       /* Clock source Fcyc, trigger mode 1, Mode 0 (disable  PWM) */

    /* configure pins */
    _PCFG8 = 1;             /* PWM 1 is digital */
    _TRISB8 = 0;            /* PWM 1 is an output */
    _LATB8 = 0;             /* drive output low */

    __builtin_write_OSCCONL(OSCCON & 0xBF);     /* unlock Peripheral Pin Select Registers */
    _RP8R = 17 + 1;                             /* PWM offset + channel */
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock Peripheral Pin Select Registers */

    /* turn module on */
    OC1CON1 |= 0x6;

} // end of initPWM

/* Sets the duty cycle of PWM 1 to that of the passed value (as an integer percent) */
static void setPwm1DutyCycle(unsigned newDutyCycle)
{
    if(newDutyCycle > MAX_DUTYCYCLE)    /* if the passed value is too large */
        newDutyCycle = MAX_DUTYCYCLE;   /* set the value to the largest allowable duty cycle */

    /* now set the duty cycle register to the new duty cycle value scaled to the current frequency */
    OC1R = round(((unsigned long)(OC1RS + 1) * newDutyCycle) / (float)MAX_DUTYCYCLE);

} // end of setPWMDutyCycle

/* Initializes the OC module */
static void initPwm2(void)
{
    /* disable module before configuring */
    OC2CON1 &= 0xfff8;

    /* OC config */
    OC2RS = FCY/5000 - 1;   /* set the PWM period to a frequency of 5kHz */
    OC2R  = 0;              /* set the Duty Cycle to 0% */
    OC2CON2 = 0x001F;       /* set PWM 2 to sync with itself, non inverted signal (active high) */
    OC2CON1 = 0x1C08;       /* Clock source Fcyc, trigger mode 1, Mode 0 (disable  PWM) */

    /* configure pins */
    _PCFG9 = 1;             /* PWM 2 is digital */
    _TRISB9 = 0;            /* PWM 2 is an output */
    _LATB9 = 0;             /* drive output low */

    __builtin_write_OSCCONL(OSCCON & 0xBF);     /* unlock Peripheral Pin Select Registers */
    _RP9R = 17 + 2;                             /* PWM offset + channel */
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock Peripheral Pin Select Registers */

    /* turn module on */
    OC2CON1 |= 0x6;

} // end of initPWM

/* Sets the duty cycle of PWM 2 to that of the passed value (as an integer percent) */
static void setPwm2DutyCycle(unsigned newDutyCycle)
{
    if(newDutyCycle > MAX_DUTYCYCLE)    /* if the passed value is too large */
        newDutyCycle = MAX_DUTYCYCLE;   /* set the value to the largest allowable duty cycle */

    /* now set the duty cycle register to the new duty cycle value scaled to the current frequency */
    OC2R = round(((unsigned long)(OC2RS + 1) * newDutyCycle) / (float)MAX_DUTYCYCLE);

} // end of setPWMDutyCycle

const Led blueLed = {dutycycle:setPwm1DutyCycle, init:initPwm1},
          redLed  = {dutycycle:setPwm2DutyCycle, init:initPwm2};

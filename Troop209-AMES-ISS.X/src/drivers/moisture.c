#include "system.h"
#include "moisture.h"

static unsigned readQ1(void)
{
    AD1CON1bits.ADON = ON;     /* Turn the A/D conversion module on */

    ADC1BUF0 = 0;              /* clear the result buffer */
    AD1CHS0 = 0;               /* set channel to sample */
    AD1CON1bits.DONE = NO;     /* clear the done bit before starting */

    AD1CON1bits.SAMP = ON;	   /* start the sampling sequence */
    while (!AD1CON1bits.DONE); /* wait for conversion to finish */
    AD1CON1bits.ADON = OFF;    /* turn the A/D converter off */

    return ADC1BUF0;	         /* return the converted result */
}

static unsigned readQ2(void)
{
  AD1CON1bits.ADON = ON;     /* Turn the A/D conversion module on */

    ADC1BUF0 = 0;              /* clear the result buffer */
    AD1CHS0 = 1;               /* set channel to sample */
    AD1CON1bits.DONE = NO;     /* clear the done bit before starting */

    AD1CON1bits.SAMP = ON;	   /* start the sampling sequence */
    while (!AD1CON1bits.DONE); /* wait for conversion to finish */
    AD1CON1bits.ADON = OFF;    /* turn the A/D converter off */

    return ADC1BUF0;	         /* return the converted result */
}

static unsigned readQ3(void)
{
    AD1CON1bits.ADON = ON;     /* Turn the A/D conversion module on */

    ADC1BUF0 = 0;              /* clear the result buffer */
    AD1CHS0 = 2;               /* set channel to sample */
    AD1CON1bits.DONE = NO;     /* clear the done bit before starting */

    AD1CON1bits.SAMP = ON;	   /* start the sampling sequence */
    while (!AD1CON1bits.DONE); /* wait for conversion to finish */
    AD1CON1bits.ADON = OFF;    /* turn the A/D converter off */

    return ADC1BUF0;	         /* return the converted result */
}

static unsigned readQ4(void)
{
  AD1CON1bits.ADON = ON;     /* Turn the A/D conversion module on */

    ADC1BUF0 = 0;              /* clear the result buffer */
    AD1CHS0 = 3;               /* set channel to sample */
    AD1CON1bits.DONE = NO;     /* clear the done bit before starting */

    AD1CON1bits.SAMP = ON;	   /* start the sampling sequence */
    while (!AD1CON1bits.DONE); /* wait for conversion to finish */
    AD1CON1bits.ADON = OFF;    /* turn the A/D converter off */

    return ADC1BUF0;	         /* return the converted result */
}

static unsigned sampleQ1(int samplesToTake, int interval)
{
    // sample the sensor multiple times for validation
    Uint32 readings = 0;  // accumulator
    int i;                // itterator

    for(i = 0; i < samplesToTake; ++i)
    {
        readings += readQ1();
        wait(interval);
    }

    // average the readings and return it
    return (unsigned)(readings / samplesToTake);
}

static unsigned sampleQ2(int samplesToTake, int interval)
{
    // sample the sensor multiple times for validation
    Uint32 readings = 0;  // accumulator
    int i;                // itterator

    for(i = 0; i < samplesToTake; ++i)
    {
        readings += readQ2();
        wait(interval);
    }

    // average the readings and return it
    return (unsigned)(readings / samplesToTake);
}

static unsigned sampleQ3(int samplesToTake, int interval)
{
    // sample the sensor multiple times for validation
    Uint32 readings = 0;  // accumulator
    int i;                // itterator

    for(i = 0; i < samplesToTake; ++i)
    {
        readings += readQ3();
        wait(interval);
    }

    // average the readings and return it
    return (unsigned)(readings / samplesToTake);
}

static unsigned sampleQ4(int samplesToTake, int interval)
{
    // sample the sensor multiple times for validation
    Uint32 readings = 0;  // accumulator
    int i;                // itterator

    for(i = 0; i < samplesToTake; ++i)
    {
        readings += readQ4();
        wait(interval);
    }

    // average the readings and return it
    return (unsigned)(readings / samplesToTake);
}

/**
 * Initializes moisture analog ports
 */
static void initialize(void)
{
    /*  Channel Pin Configuration
     *
     *  The pins need to be configured as analog inputs.
     */
    _TRISB0 = 1; /* Q1 - configure port as input */
    _PCFG0  = 0; /* Q1 - pin is in analog mode (disable digital mode) */
    _TRISB1 = 1; /* Q2 */
    _PCFG1  = 0; /* Q2 */
    _TRISB2 = 1; /* Q3 */
    _PCFG2  = 0; /* Q3 */
    _TRISB3 = 1; /* Q4 */
    _PCFG3  = 0; /* Q4 */
}

const Moisture moisture = {readQ1:readQ1, readQ2:readQ2, readQ3:readQ3, readQ4:readQ4, getQ1:sampleQ1, getQ2:sampleQ2, getQ3:sampleQ3, getQ4:sampleQ4, init:initialize};

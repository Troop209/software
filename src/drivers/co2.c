#include "system.h"
#include "co2.h"

static unsigned readValue(void)
{
    AD1CON1bits.ADON = ON;     /* Turn the A/D conversion module on */

    ADC1BUF0 = 0;              /* clear the result buffer */
    AD1CHS0 = 4;               /* set channel to sample */
    AD1CON1bits.DONE = NO;     /* clear the done bit before starting */

    AD1CON1bits.SAMP = ON;	   /* start the sampling sequence */
    while (!AD1CON1bits.DONE); /* wait for conversion to finish */
    AD1CON1bits.ADON = OFF;    /* turn the A/D converter off */

    return ADC1BUF0;	       /* return the converted result */
}

static void setHeaterOutput(boolean desiredOutputState)
{
    _TRISE3 = 0;    /* configure port as output */
    _RE3 = desiredOutputState; /* set the output */
}

//static void warmUp(void)
//{
//    setPowerOutput(ON);   // turn module power on
//    setHeaterOutput(ON);  // turn heating element on
//    wait(1000);           // wait for the sensor to warm up
//}

static void turnOff(void)
{
    system.off6volt();     // turn 6volt enable off
    setHeaterOutput(OFF);  // turn heating element off
}

static void turnOn(void)
{
    system.on6volt();     // turn 6volt enable on
    setHeaterOutput(ON);  // turn heating element on
}

static unsigned getReading(int samplesToTake, int interval)
{
    // sample the sensor multiple times for validation
    Uint32 readings = 0;  // accumulator
    int i;                // iterator

    for(i = 0; i < samplesToTake; ++i)
    {
        readings += readValue();
        wait(interval);
    }

    // average the readings and return it
    return (unsigned)(readings / samplesToTake);

}

static boolean readTest(void)
{
    _TRISE2 = 1; /* configure port as input */
    return _RE2; /* return the value on the pin */
}

static void initialize(void)
{
    /*  Channel Pin Configuration
     *
     *  The pins need to be configured as analog inputs.
     */
    _TRISB4 = 1; /* configure port as input */
    _PCFG4  = 0; /* pin is in analog mode (disable digital mode) */

    system.off6volt();
    setHeaterOutput(OFF);
}

const Co2 co2 = {status:readTest, read:readValue, get:getReading, on:turnOn, off:turnOff, setHeater:setHeaterOutput, init:initialize};

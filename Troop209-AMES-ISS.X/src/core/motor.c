#include "system.h"
#include "motor.h"


/**
 * initPwm() Information
 *
 * This function initializes output compare module 1 and configures it to output
 * a pulse width modulated signal.
 */
 static void initPwm9(void)
{
    /* disable module before configuring */
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR8=35  ;   //Select oc 9  as RP16
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    T4CON       = 0 ;   // clear everything out including t32 "Gang counter" bit
    T5CON       = 0 ;   // clear everything out
    T5CON       = 0x0030 ;  // TImer 5 enable, /256, interrnal 16G clock
    nop()                ;  // let hardcware catch up to software (1 instruiction cycle))
    PR5         =   3125 ;  // 20 Hz POeriod  as 16000000/256/3124=20.00000 // /256 is prescaler  /3125 is compare value
    nop()                ;  // and 1 extra 
    T5CON       = 0x8030 ;  // TImer 5 enable, /256, interrnal 16G clock
    nop()                ;  // let hardcware catch up to software (1 instruiction cycle))

    OC9CON1 &= 0xfff8;

    /* OC config */
    // OC9CON1 = 0x1C08;     // clock source Fcy, trigger mode 1, disabled
    OC9CON1 = 0x1C08;        // clock source Tmr5, trigger mode 1, disabled
    OC9CON2 = 0x008F;        // triggered, non inverted signal (active high)
    setPwm9Period(3125)  ;
    setPwm9DutyCycle(24000)  ;
    
    /* configure pins */
    _PCFG8  = 1;  // PWM 1 is digital and NOT Analog input           
    _TRISF3 = 0;  // PWM 1 is an output
    _LATF3  = 0;  // drive output low

    /* turn module on */
    OC9CON1 |= 0x5 ;
}

/**
 * setPwm1DutyCycle() Information
 *
 * This function sets the duty cycle of PWM 1 to that of the passed value (as
 * a positive integer percent). Values that are larger than MAX_DUTYCYCLE are
 * coerced to the max limit.
 */
 static void setPwm9DutyCycle(Uint16 newDutyCycle)
{
     /* (KYu - need a MAX_DUTYCYCLE value)
    if(newDutyCycle > MAX_DUTYCYCLE)  // if the passed value is too large
       newDutyCycle = MAX_DUTYCYCLE; // set value to max allowable duty cycle
*/
    OC9RS   =   newDutyCycle;
    OC9R    =   1;  // (KYu) why is this 1? 
}

static void setPwm9Period(Uint16 newPeriod)
{
    PR5   = newPeriod  ;   // Write Period Value into TIMER5 Preset Register
}

const Motor motor = {dutycycle:setPwm9DutyCycle, period:setPwm9Period, init:initPwm9};

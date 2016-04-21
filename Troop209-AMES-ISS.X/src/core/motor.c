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
    
    OC9RS   =   24000;
    OC9R    =   1;  
    
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
    OC9R    =   1; 
}

static void setPwm9Period(Uint16 newPeriod)
{
    PR5   = newPeriod  ;   // Write Period Value into TIMER5 Preset Register
}

// Need to update these calues based on new motor to work
int PWM_Positions[18] = {16000, 17000, 18000, 19000, 20000, 21000, 22000, 23000, 24000,
        25000, 26000, 27000, 28000, 29000, 30000, 31000, 32000, 16000};

static void setPwm9Position(Uint16 position) {
    setPwm9DutyCycle(PWM_Positions[position]);
    delay(10000);
}

void    SET_PWM9_JOG(int jog) 
{   // add an offset to the current motor position 
     // intended to do small move until Align switch makes 
     OC9R=OC9R+jog ; 
     return ; 
}    

    
const Motor motor = {dutycycle:setPwm9DutyCycle, period:setPwm9Period, init:initPwm9, positionTo:setPwm9Position};

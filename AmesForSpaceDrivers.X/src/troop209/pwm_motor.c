/**
 * Non PWM LED (aka standard LED)
 * Copyright (C) 2016 Pathway Council - Troop 209
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "system.h"
#include "pwm_motor.h"

void initPWM(void)
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
    SET_PWM9_Period( 3125)  ;
    SET_PWM9_DutyCycle( 24000)  ;
    
    /* configure pins */
    _PCFG8  = 1;  // PWM 1 is digital and NOT Analog input           
    _TRISF3 = 0;  // PWM 1 is an output
    _LATF3  = 0;  // drive output low

    /* turn module on */
    OC9CON1 |= 0x5 ;
}

int SET_PWM9_Period( int Period)
{   PR5   = Period  ;   // Write Period Value into TIMER5 Preset Register
    return  (0)     ;
}

int SET_PWM9_DutyCycle( int dutycyc)
{   OC9RS   =   dutycyc    ;
    OC9R    =   1          ; 
    
    return  (0)     ;
}

int writeMotorPositionAbs(int MPcnt)
{   // convert 0-4095 as percentage to pulse width & set
    // Argument is relative change from current position
    OC9RS = ((MPcnt*4) + 16000)   ;
    return (MPcnt)    ;
}

/**
 * ledX Information
 *
 * ledX are the software wrappers for the NESI LEDs. They use a pulse width
 * modulated signal to control the LED banks. ledR controls the R bank
 * and ledB controls the B bank.
 */
const Motor
    motor = {init : initPWM, turn : writeMotorPositionAbs};

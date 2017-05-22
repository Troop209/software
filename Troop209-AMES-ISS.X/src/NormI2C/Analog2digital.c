/**
 * NESI Resistive Sensor Driver License
 * Copyright (C) 2013 Mickie Byrd
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

/**
 * Change Log
 *
 * 09/24/2013 - Dakotah Karrer
 *   Renamed and refactored from "moisture" to "resistiveSensors".
 */

#include "system.h"
// #include "resistiveSensors.h"

/**
 * readQ1() Information
 *
 * This function samples the sensor connected to quadrant 1 (Q1) once and
 * returns the raw 10-bit value.from channel AN0
 */
// static 
Uint16 readQ1(void) {
    AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 0;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
}

/**
 * readQ2() Information
 *
 * This function samples the sensor connected to quadrant 2 (Q2) once and
 * returns the raw 10-bit value.from channel AN1
 */
//static 
Uint16 readQ2(void) {
    AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 1;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
}

/**
 * readQ3() Information
 *
 * This function samples the sensor connected to quadrant 3 (Q3) once and
 * returns the raw 10-bit value.from channel AN2
 */
//static 
Uint16 readQ3(void) {
    AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 2;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
}

/**
 * readQ4() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q4) once and
 * returns the raw 10-bit value.from channel AN3
 */
// static 
Uint16 readQ4(void) {
    AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 3;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
/**
 * 
 * CHANGED FUNCTION NUMBERING SCHEME FROM CHannal +1 to Channel +0
 * 
 * readQ5() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q4) once and
 * returns the raw 10-bit value.from channel AN5
 */
/*  remove comment wrapper to use AN channel 4. Also enable input in initialize function
Also enable in putr in int function
static Uint16 readQ5(void) {
  if (( _TRISB5 = 1) && (_PCFG5  == 0))
  { AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 5;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
   }
  else
  { // Channel not enabled as an Analog Input
          return (0)    ;
  }             
}
*/
/**
 * readQ4() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q4) once and
 * returns the raw 10-bit value.from channel AN10
 */
//static 
Uint16 readC10(void) {
  if (( _TRISB10 = 1) && (_PCFG10  == 0))
  {  AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 10;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
  }
  else
  { // Channel not enabled as an Analog Input
          return (0)    ;
  }             
}

/**
 * readQ4() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q4) once and
 * returns the raw 10-bit value.from channel AN11
 */

/*  remove comment wrapper to use AN channel 4. Also enable input in initialize function
static Uint16 readQ11(void) {
  if (( _TRISB11 = 1) && (_PCFG11  == 0))
  { AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 11;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
  }
  else
  { // Channel not enabled as an Analog Input
          return (0)    ;
  }             
}
*/
/**
 * readQ4() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q4) once and
 * returns the raw 10-bit value.from channel AN12
 */

/*  remove comment wrapper to use AN channel 4. Also enable input in initialize function
static Uint16 readQ12(void) {
  if (( _TRISB12 = 1) && (_PCFG12  == 0))
  { AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 12;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
  }
  else
  { // Channel not enabled as an Analog Input
          return (0)    ;
  }             
}
*/

/**
 * readQ4() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q4) once and
 * returns the raw 10-bit value.from channel AN13
 */

/*  remove comment wrapper to use AN channel 4. Also enable input in initialize function
static Uint16 readQ13(void) {
  if (( _TRISB13 = 1) && (_PCFG13  == 0))
  { AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUF0 = 0;              // clear the result buffer
    AD1CHS0 = 13;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE); // wait for conversion to finish
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return ADC1BUF0;           // return the converted result
  }
  else
  { // Channel not enabled as an Analog Input
          return (0)    ;
  }             
}
 * */

/**
 * sampleQ1() Information
 *
 * This function samples the sensor connected to quadrant 1 (Q1) and returns
 * the average raw 10-bit value.
 */
static Uint16 sampleQ1(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readQ1();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}

/**
 * sampleQ2() Information
 *
 * This function samples the sensor connected to quadrant 2 (Q2) and returns
 * the average raw 10-bit value.
 */
static Uint16 sampleQ2(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readQ2();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}

/**
 * sampleQ3() Information
 *
 * This function samples the sensor connected to quadrant 3 (Q3) and returns
 * the average raw 10-bit value.
 */
static Uint16 sampleQ3(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readQ3();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}

/**
 * sampleQ4() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q4) and returns
 * the average raw 10-bit value.
 */
static Uint16 sampleQ4(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readQ4();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}

/**
 * sampleQ5() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q5) and returns
 * the average raw 10-bit value.
 */

/* Remove comment wrapp[r from this function and corre3sponding ReadQ function. Enable A/D input in initialize
static Uint16 sampleQ5(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readQ5();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}
*/

/**
 * sampleQ10() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q10) and returns
 * the average raw 10-bit value.
 */

// /* Remove comment wrapp[r from this function and corre3sponding ReadQ function. Enable A/D input in initialize
static Uint16 sampleQ10(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readC10();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}
// */

/**
 * sampleQ11() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q11) and returns
 * the average raw 10-bit value.
 */

/* Remove comment wrapp[r from this function and corre3sponding ReadQ function. Enable A/D input in initialize
static Uint16 sampleQ11(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readQ11();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}
*/

/**
 * sampleQ12() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q12) and returns
 * the average raw 10-bit value.
 */

/* Remove comment wrapp[r from this function and corre3sponding ReadQ function. Enable A/D input in initialize
static Uint16 sampleQ12(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readQ12();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}
 */

/**
 * sampleQ13() Information
 *
 * This function samples the sensor connected to quadrant 4 (Q13) and returns
 * the average raw 10-bit value.
 */
/* Remove comment wrapp[r from this function and corre3sponding ReadQ function. Enable A/D input in initialize

static Uint16 sampleQ13(int samplesToTake, int interval) {
    // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) {
        readings += readQ13();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}
*/

/**
 * initialize() Information
 *
 * This function initializes hardware needed by the module.
 */
// static void initialize(void) {
//static 
void initA2D(void) {
    /**
     * Channel Pin Configuration
     *
     * The pins need to be configured as analog inputs.
     */
    _TRISB0 = 1; // Q1 - configure port as input
    _PCFG0  = 0; // Q1 - pin is in analog mode (disable digital mode)
    _TRISB1 = 1; // Q2  AN1
    _PCFG1  = 0; // Q2  AN1
    _TRISB2 = 1; // Q3  AN2
    _PCFG2  = 0; // Q3  AN2
    _TRISB3 = 1; // Q4  AN3
    _PCFG3  = 0; // Q4  AN3
    //  _TRISB5 = 1; // Q5   AN5     // default to disabled
    // __PCFG5  = 0; // Q5   AN5     // default to disabled
    _TRISB10 = 1; // Q10  AN10        // PWM Motor Current
    _PCFG10  = 0; // Q10  AN10        // PWM Motor Current
    // __TRISB11 = 1; // Q11  AN11     // default to disabled
    // __PCFG11  = 0; // Q11  AN11     // default to disabled
    // __TRISB12 = 1; // Q12  AN12     // USED AS Switch Input- Do Not use as Analog
    // __PCFG12  = 0; // Q12  AN12     // USED AS Switch Input- Do Not use as Analog
    // __TRISB13 = 1; // Q13  AN13     // default to disabled
    // __PCFG13  = 0; // Q13  AN13     // default to disabled
}
void unused_Calls   (void)
{sampleQ1(4, 1) ;
 sampleQ2(4, 1) ;
 sampleQ3(4, 1) ;
 sampleQ4(4, 1) ;
 sampleQ10(4, 1) ;
 return ;
 }
 
/**
 * resistiveSensors Information
 *
 * resistiveSensors is the software wrapper for the NESI Resistive Sensor
 * interfaced.
 */
/*
const ResistiveSensors resistiveSensors = {
    readQ1  : readQ1,
    readQ2  : readQ2,
    readQ3  : readQ3,
    readQ4  : readQ4,
    readQ5  : readQ5,
    readQ10 : readQ10,
    readQ11 : readQ11,
    readQ12 : readQ12,
    readQ13 : readQ13,
    getQ1   : sampleQ1,
    getQ2   : sampleQ2,
    getQ3   : sampleQ3,
    getQ4   : sampleQ4,
    getQ5   : sampleQ5,
    getQ10  : sampleQ10,
    getQ11  : sampleQ11,
    getQ12  : sampleQ12,
    getQ13  : sampleQ13,
    init : initialize
};
*/

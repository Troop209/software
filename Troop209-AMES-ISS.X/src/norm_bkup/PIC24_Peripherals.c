
#include <p24FJ256GB106.h>

#include "system.h"
#include "nesi.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *      Pulse Width Modulation (PWM) 
 *       Servo Motor positioning
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/

#define MIN_DUTYCYCLE   (0)
#define MAX_DUTYCYCLE   (100)
/*
 * setPwm1DutyCycle() Information
 *
 * This function sets the duty cycle of PWM 1 to that of the passed value (as
 * a positive integer percent). Values that are larger than MAX_DUTYCYCLE are
 * coerced to the max limit.
 */
 void setPwm9DutyCycle(Uint16 newDutyCycle)
{
    if(newDutyCycle > MAX_DUTYCYCLE)  // if the passed value is too large
        newDutyCycle = MAX_DUTYCYCLE; // set value to max allowable duty cycle

    /* now set the duty cycle register to the new duty cycle value scaled to the current frequency */
    OC9RS = round(((Uint32)(OC9RS + 1) * newDutyCycle) / (float)MAX_DUTYCYCLE);
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

void    SET_PWM9_JOG(int jog)
{   // add an offset to the current motor position
    // intended to do small move until Align switch makes
    OC9R=OC9R+jog ;
    return ;
}   


void PWM9_init(void)
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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *      Analog to Digital Converter
 *          Initialize and Channel 10 implementation
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/

/*
 * initAnalog() Information
 *
 * This function initializes microcontroller's analog module.
 */
void Analog_init(void)
{
    /**
     * Disable Module
     *
     * Before configuring the module, it will be disabled. This will avoid
     * sporadic and undefined behavior.
     */
    AD1CON1bits.ADON = 0; // Turn the A/D converter off while configuring
    
    AD1CON1 = 0 ;
    AD1CON2 = 0 ;
    AD1CON3 = 0 ;

    /**
     * General Analog to Digital Conversion Module Configuration
     *
     * The ADC module will be configured to sample the chosen channel when
     * triggered.
     */
    AD1CON1bits.ADSIDL = 0;   // do not stop ADC in Idle mode
    AD1CON1bits.FORM = 0b00;  // data formated as an unsigned integer
    AD1CON3bits.ADRC = 0;     // ADC drives clock from the system clock
    AD1CON1bits.SSRC = 0b111; // autoconvert samples
    AD1CON1bits.ASAM = 0;     // manual sample start
    AD1CON3bits.SAMC = 4;     // ADC samples for 4 Tad
    AD1CON3bits.ADCS = 1;     // ADC clock is 2 Tcy

    /**
     * ADC Input Configuration
     *
     * Inputs will be read as needed. Scanning will be disabled by the ADC module.
     */
    AD1CHS0bits.CH0NA = 0;    // MUXA negative input is Vref-
    AD1CON2bits.VCFG = 0b000; // ADC Vref+ reference is AVdd and Vref- is AVss
    AD1CON2bits.ALTS = 0;     // always sample from input A (mux A)
    AD1CON2bits.CSCNA = 0;    // do not scan ADC inputs on CH0+ during sample A
    AD1CSSL = 0x0400 ;              // disable all channel scans except 10
    AD1PCFGL= 0x0400 ;   

    /**
     * Interrupt Configuration
     *
     * Interrupts will not be used for conversion and storage.
     */
    IFS0bits.AD1IF = 0;  // clear the A/D interrupt flag
    IEC0bits.AD1IE = 0;  // disable A/D interrupt

}
/*
 void    A2D_c10_init (void)
{   _TRISB10 = 1; // Q10  AN10        // PWM Motor Current
    _PCFG10  = 0; // Q10  AN10        // PWM Motor Current
    AD1CON1  = 0x00E4   ;
    AD1CON2  = 0x043C   ;
    AD1CON3  = 0x1F00   ;
    return      ;
}
*/
  
/**
 * readQ4() Information
 *
 * This function samples the input connected to A/D input 10 once and
 * returns the raw 10-bit value.from channel AN10
 */
Uint16 A2D_c10_read(void) 
{  if (( _TRISB10 = 1) && (_PCFG10  == 1))
  { int AD_Chnl_10 = 0  ;
    AD1CON1bits.ADON = ON;     // Turn the A/D conversion module on

    ADC1BUFA = 0;              // clear the result buffer
    AD1CHS0 = 10;               // set channel to sample
    AD1CON1bits.DONE = NO;     // clear the done bit before starting

    AD1CSSL = 0x0400 ;              // disable all channel scans except 10
    AD1PCFGLbits.PCFG10 = 0 ;       // config as analog input

    AD1CON1bits.SAMP = ON;     // start the sampling sequence
    while (!AD1CON1bits.DONE) {}; // wait for conversion to finish
    AD_Chnl_10 = ADC1BUFA;  
    AD1CON1bits.ADON = OFF;    // turn the A/D converter off

    return AD_Chnl_10 ;           // return the converted result
  }
  else
  { // Channel not enabled as an Analog Input
          return (0)    ;
  }
}

  /**
 * sampleC10() Information
 *
 * This function samples the sensor connected to input connected to A/D input 10 and returns
 * the average raw 10-bit value.
 */

// 
Uint16 A2D_c10_sample(int samplesToTake, int interval) 
{   // sample the sensor multiple times for validation
    Uint32 readings = 0; // accumulator
    int i; // iterator

    for (i = 0; i < samplesToTake; ++i) 
    {   readings += A2D_c10_read();
        wait(interval);
    }

    // average the readings and return it
    return (Uint16) (readings / samplesToTake);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *       Pulse Width Modulation (PWM)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/

// const ReadServoAmps readServoAmps  = { read_C10 : read_C10, getC10  : sample_C10, init: A2D_C10_init } ;
 int RSP78_init(void) { // Flush everything from before
    IC7CON1 = 0;
    IC7CON2 = 0;
    IC8CON1 = 0;
    IC8CON2 = 0;
    //Set the input source as RP27

    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPINR10 = 27;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    IC8CON2bits.IC32 = 1; // pull two units apart for now
    IC7CON2bits.IC32 = 1;
    IC8CON1bits.ICTSEL = 7;
    IC7CON1bits.ICTSEL = 7; // clock src 5=timer 5; 7 = FCY (16MHz))
    IC8CON2bits.SYNCSEL = 0;
    IC7CON2bits.SYNCSEL = 0;
    IC8CON2bits.ICTRIG = 0;
    IC7CON2bits.ICTRIG = 0;
    IC8CON1bits.ICI = 3; // create masked interrrupt every 4th capture
    IC7CON1bits.ICI = 3; // create masked interrrupt every 4th capture
    IC8CON1bits.ICM = 5; // 3 every rising edge; 4 every 4; 5 every 16
    IC7CON1bits.ICM = 3; // 3 every rising edge; 4 every 4; 5 every 16
    return (0);
}

long int RSP78_Read_Sig_Period(void) // Abbreviate as RSP
{
    long int RSP78_PerVal = 0; //. result value
    long int RSP78_tempval = 0; // build a 32 bit value
    long int RSP78_temphalf = 0;
    long int RSP78_temparray[5] = {0, 0, 0, 0, 0};
    long int RSP78_cnt = 0;
    int i = 0;
    int j = 0;

    // Calcs assume a partially full FIFO
    // Gathers 2 (minimum ) or up to 8 FIFO readings (if FIFO filling as fast as we cleaning. 4 max typical)
    // ets the readings, saves in array, calcs, differential, converts to NANOSECONDS

    // RETURNS PERIOD of signal (Average delta counter values * clock pulse width)

    // flush old readings in FIFO
    while (IC7CON1bits.ICBNE == 1) {
        RSP78_tempval = IC8BUF;
        RSP78_tempval = IC7BUF;
    }
    // FIFO clean, now get 5 new readings from input signal
    // Note counter is NOT reset on input edge, so must calc differences 
    while (RSP78_cnt < 5) {
        while (IC7CON1bits.ICBNE == 0) { // wait for next reading or continue if present
        }
        // get a single reading
        RSP78_tempval = 0;
        RSP78_tempval = IC8BUF;
        RSP78_tempval = RSP78_tempval * 65536; // push to upper word
        RSP78_tempval = RSP78_tempval + IC7BUF; // add lsb
        RSP78_temparray[RSP78_cnt] = RSP78_tempval;
        if (IC8CON1bits.ICOV == 1) {
            RSP78_cnt = -1; // set count so will be 0 after next increment
        }
        RSP78_cnt++;
    }
    // have five contiguous readings in data array.
    // Now calculate 4 differential readings (2-1, 3-2, 4-3, 5-4)
    // And accumulate differential total

    // calc differential values
    for (i = 0; i < RSP78_cnt - 1; i++) {
        j = i + 1;
        RSP78_temparray[i] = RSP78_temparray[j] - RSP78_temparray[i]; // the differentail value
        RSP78_PerVal = RSP78_PerVal + RSP78_temparray[i]; // totalized value
    }

    // Now have pulse width in clock counts, but lets convert to nanoseconds
    // Clock is 16MHz, so need to multiply average count * 1/16Mhz (62.5nS)
    // Had trouble getting long int divisions to work,
    RSP78_PerVal = RSP78_PerVal >> 2; //  Mathmatically equal to value / 4
    RSP78_temphalf = RSP78_PerVal >> 1; // get value * 0.5
    RSP78_PerVal = RSP78_PerVal * 62; // get valuye * 62 
    RSP78_PerVal = RSP78_PerVal + RSP78_temphalf; // now get value *62+ value * .5 or value *62.5

    return (RSP78_PerVal);
}


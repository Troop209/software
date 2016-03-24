
#include <p24FJ256GB106.h>

#include "system.h"
#include "nesi.h"


int RSP78_init (void)
{  // Flush everything from before
    IC7CON1=0   ;
    IC7CON2=0   ;
    IC8CON1=0   ;
    IC8CON2=0   ;
    //Set the input source as RP27
    
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
      RPINR10=27  ;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers
    
    IC8CON2bits.IC32    = 1  ;  // pull two units apart for now
    IC7CON2bits.IC32    = 1  ;
    IC8CON1bits.ICTSEL  = 7  ;  
    IC7CON1bits.ICTSEL  = 7  ;    // clock src 5=timer 5; 7 = FCY (16MHz))
    IC8CON2bits.SYNCSEL = 0  ;
    IC7CON2bits.SYNCSEL = 0  ;
    IC8CON2bits.ICTRIG  = 0  ;
    IC7CON2bits.ICTRIG  = 0  ;      
    IC8CON1bits.ICI     = 3  ;    // create masked interrrupt every 4th capture
    IC7CON1bits.ICI     = 3  ;    // create masked interrrupt every 4th capture
    IC8CON1bits.ICM     = 5  ;    // 3 every rising edge; 4 every 4; 5 every 16
    IC7CON1bits.ICM     = 3  ;    // 3 every rising edge; 4 every 4; 5 every 16
    return (0)      ;
}

long int RSP78_Read_Sig_Period(void)     // Abbreviate as RSP
{   long int RSP78_PerVal = 0    ;   //. result value
    long int RSP78_tempval=0      ;   // build a 32 bit value
    long int RSP78_temphalf = 0   ;
    long int RSP78_temparray[5]   ={0,0,0,0,0}  ;
    long int RSP78_cnt=0               ;
    int i = 0               ;
    int j = 0               ;
    
   // Calcs assume a partially full FIFO
   // Gathers 2 (minimum ) or up to 8 FIFO readings (if FIFO filling as fast as we cleaning. 4 max typical)
   // ets the readings, saves in array, calcs, differential, converts to NANOSECONDS
    
   // RETURNS PERIOD of signal (Average delta counter values * clock pulse width)
    
     // flush old readings in FIFO
     while (IC7CON1bits.ICBNE == 1)
    { RSP78_tempval = IC8BUF ;
      RSP78_tempval = IC7BUF ;
    }
   // FIFO clean, now get 5 new readings from input signal
   // Note counter is NOT reset on input edge, so must calc differences 
    while (RSP78_cnt <5)
    {   while (IC7CON1bits.ICBNE == 0)
        {   // wait for next reading or continue if present
        }
        // get a single reading
        RSP78_tempval=0   ;
        RSP78_tempval = IC8BUF ;
        RSP78_tempval = RSP78_tempval*65536    ;   // push to upper word
        RSP78_tempval = RSP78_tempval+IC7BUF   ;   // add lsb
        RSP78_temparray[RSP78_cnt]=RSP78_tempval     ;
        if(IC8CON1bits.ICOV==1)
        { RSP78_cnt=-1          ;   // set count so will be 0 after next increment
        }
        RSP78_cnt++                   ;
    }
    // have five contiguous readings in data array.
    // Now calculate 4 differential readings (2-1, 3-2, 4-3, 5-4)
    // And accumulate differential total
    
    // calc differential values
    for (i=0; i<RSP78_cnt-1; i++)
    {   j=i+1                       ;
        RSP78_temparray[i]=RSP78_temparray[j]-RSP78_temparray[i]  ;   // the differentail value
        RSP78_PerVal=RSP78_PerVal + RSP78_temparray[i]          ;   // totalized value
    }

    // Now have pulse width in clock counts, but lets convert to nanoseconds
    // Clock is 16MHz, so need to multiply average count * 1/16Mhz (62.5nS)
    // Had trouble getting long int divisions to work,
    RSP78_PerVal=RSP78_PerVal >> 2    ;  //  Mathmatically equal to value / 4
    RSP78_temphalf=RSP78_PerVal >> 1   ;  // get value * 0.5
    RSP78_PerVal=RSP78_PerVal * 62   ;   // get valuye * 62 
    RSP78_PerVal=RSP78_PerVal + RSP78_temphalf  ;  // now get value *62+ value * .5 or value *62.5
        
    return (RSP78_PerVal)            ;
}

void PWM9_init (void)
{   T4CON       = 0 ;   // clear everything out including t32 "Gang counter" bit
    T5CON       = 0 ;   // clear everything out
    T5CON       = 0x0030 ;  // TImer 5 enable, /256, interrnal 16G clock
    nop()                ;  // let hardcware catch up to software (1 instruiction cycle))
    PR5         =   3124 ;  // 20 Hz POeriod
    nop()                ;  // and 1 extra 
    T5CON       = 0x8030 ;  // TImer 5 enable, /256, interrnal 16G clock
    nop()                ;  // let hardcware catch up to software (1 instruiction cycle))
    nop()                ;  // and 1 extra 
 
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
      RPOR8=35  ;   //Select oc 9  as RP16
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers
    
    OC9CON1     = 0x1C08 ;
    nop()                ;
    OC9CON2     = 0x008F ;
    nop()                ;
    OC9RS       =  24000 ;
    OC9R        =  56000 ;
    OC9CON1     = 0x1C0A ;      // All set up. Now Go
    nop()                ;
           
    /*
     * T5CONbits.TON   = 1 ;   // Timer 5 on
    T5CONbits.TSIDL = 0 ;   // Timer 5 on
    T5CONbits.TGATE = 0 ;   // Timer 5 on
    T5CONbits.TCKPS = 3 ;   // Timer 5 ./. 256 prescaler
    // T5CONbits.T32   = 0 ;   // Timer 5 16 bi8t mode
    T5CONbits.TCS   = 0 ;   // Timer 5
  
    PR5=3124    ;
    // saet up oc9
    OC9CON1 &=  0xFFF8  ;   // DUty Cycle counter
    
    OC9CON1=0x1C08       ;
    OC9CON2=0x008F       ;  // to timer 5
    
    OC9RS   = 64000  ;
    OC9R    =    24000  ;
    
    _TRISF3  =   0   ;
    _LATF3   =   0   ;
    
    
    OC9CON1 |=  0x6 ;
     */ 
    return  ;
}    

int SET_PWM9_Period( int Period)
{    PR5    = Period  ;   // write lower word to 7. andED off upper 16 bits
    return  (0)     ;
}

int SET_PWM9_DutyCycle( int dutycyc)
{   OC9RS   =   dutycyc    ;
    
    return  (0)     ;
}

void    SET_PWM9_JOG(int jog)
{   // add an offset to the current motor position
    // intended to do small move until Align switch makes
    OC9R=OC9R+jog ;
    return ;
}   

/*
long int SET_PWM78_Get_Amps(long int dutycyc)
{   long int val=0 ;
    int cnt=0   ;
    SET_PWM78_DutyCycle(dutycyc)    ;
    while (PORTFbits.6 == ON)    { } ;

    while (1)
    {  val=val + readQ10 ;
       cnt++             ;
       if (readQ10 < 50 )
       { val= div(val/cnt)  ;
         break ;
       }
    }
       return (val)    ;
}
*/


 void    A2D_c10_init (void)
{   _TRISB10 = 1; // Q10  AN10        // PWM Motor Current
    _PCFG10  = 0; // Q10  AN10        // PWM Motor Current
    return      ;
}
/**
 * readQ4() Information
 *
 * This function samples the input connected to A/D input 10 once and
 * returns the raw 10-bit value.from channel AN10
 */
Uint16 A2D_c10_read(void) 
{  if (( _TRISB10 = 1) && (_PCFG10  == 0))
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

// const ReadServoAmps readServoAmps  = { read_C10 : read_C10, getC10  : sample_C10, init: A2D_C10_init } ;
 
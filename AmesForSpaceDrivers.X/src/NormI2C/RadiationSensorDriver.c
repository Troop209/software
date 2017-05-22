#include "system.h"
#include "nesi.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *       Pulse Width Modulation (PWM)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/

// const ReadServoAmps readServoAmps  = { read_C10 : read_C10, getC10  : sample_C10, init: A2D_C10_init } ;
 int initRadiation(void) { // Flush everything from before
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
    IC8CON1bits.ICI = 0; // create masked interrrupt every capture
    IC7CON1bits.ICI = 0; // create masked interrrupt every capture
    IC8CON1bits.ICM = 3; // 3 - 3 every rising edge; 4 every 4; 5 every 16
    IC7CON1bits.ICM = 3; // 3- 3  every rising edge; 4 every 4; 5 every 16
    return (0);
}
 
 
//unsigned int Rotation[64]  =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}   ;
//int            Rptr =   0   ;

int readRadiation(void)
{   extern unsigned int SNS_RadPeriod     ;

    int flag    = 0 ;     
    unsigned long   period          = 0 ;       // internal time value in counts
    unsigned long   Rising          = 0 ;       // static variables !!!
    unsigned long   prevRising      = 0 ;
    unsigned int    IC7temp         = 0 ;
    unsigned int    IC8temp         = 0 ;
//  unsigned int Rotation[64]  =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}   ;

    // new value from rising edges?
    if( (IC7CON1bits.ICBNE == 1)  || (IC5CON1bits.ICOV == 1) ) 
    { // Data pending- go get. Only keep latest (last in FIFO)
        while ( (IC7CON1bits.ICBNE == 1) || (IC7CON1bits.ICOV == 1) )  
        {   prevRising  =   Rising  ;   // get last two values for delta calc
            IC7temp     =   IC7BUF  ;
            IC8temp     =   IC8BUF  ;
            Rising      =   (IC8temp * 65536) + IC7temp  ;   // value could be previous entry or multi this entry
            flag        = 1;        // no need to calc falling to rising time
        }
    }
    if (flag == 1)
    {   if(prevRising < Rising)
        { period = Rising - prevRising  ;
        } 
        else 
        { period = (Rising+4294967296) - prevRising  ;  // add 32 bit count to lesser
        }
        SNS_RadPeriod=(((long) period*(long)625)/(long) 1000) ;
        //  ic7,8               ic7,8
        // __/----------\_______/----------\_
        //   R          F
    }
    return  (SNS_RadPeriod)    ;
}

// Older drivers listed below 

long int readRad(void) // Abbreviate as RSP
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
    while ( (IC7CON1bits.ICBNE == 1)  || (IC5CON1bits.ICOV == 1) ) 
    {
        RSP78_tempval = IC8BUF;
        RSP78_tempval = IC7BUF;
    }
    // FIFO clean, now get 5 new readings from input signal
    // Note counter is NOT reset on input edge, so must calc differences 
    while (RSP78_cnt < 5) {
        while  ( (IC7CON1bits.ICBNE == 0) || (IC5CON1bits.ICOV == 1) ) 
        { // wait for next reading or continue if present
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


void initRad(void)
{   
    // configure pin for Encoder input     
    _PCFG5  = 1;  // PWM 12 is digital and NOT Analog input           
    _TRISB5 = 1;  // Encoder pin is an input
    _LATB5  = 0;  // drive output low

      // set the RPOR register so OC9 output goes to pin
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPINR9=0x1212    ;   //Select RP18 as input to both IC7 &IC8
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers
   
// set up encoder    
    // Clear contents of IC7 & IC8
    IC7CON1 = 0;        // disable for now. enable below
    IC7CON2 = 0;    
    
    IC8CON1 = 0;        // disable for now. enable below
    IC8CON2 = 0;   
  
    // Clear configure IC7 & IC8
    IC7CON1 = 0x0C00 ;        // measure period of Encoder PWM input
//   IC7CON2 = 0x0000 ;    
//    IC7CON2 = 0x000F ;        // Sync with timer 5  
    IC7CON2 = 0x0000 ;        // Sync (no trigger) with no other source  
    
    IC8CON1 = 0X0C00 ;        // measure duty cycle of Encoder PWM input
    IC8CON2 = 0x0000 ;        // measures the "On" time
   
    // turn on modules
    IC7CON1 |= 0x03 ;   // Enable Rising edge sensing
    
    IC8CON1 |= 0x02 ;   // Enable falling edge sensing
}


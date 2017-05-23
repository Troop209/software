/* 
 * File:   SensorDrivers.c
 * Author: Norman McFarland
 *
 * Created before August 31, 2016 1:34 PM
 * 
 * Changes to file:
 * Rev 1 May 19 2017
 * 1.1  Rename variables to more global format by adding Rad... prefix
 * 1.2  Move variables from inside readRadiation to outside so usable by both routines
 * 1.3  SPlit "irq" code into separate function call
 * 
 */#include "system.h"
#include "nesi.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *
 *       Pulse Width Modulation (PWM)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/
    int             RadFlag         = 0 ;     
    unsigned long   RadPeriod       = 0 ;       // internal time value in counts
    unsigned long   RadRising       = 0 ;       // static variables !!!
    unsigned long   prevRadRising   = 0 ;
    unsigned int    IC7temp         = 0 ;
    unsigned int    IC8temp         = 0 ;

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
 
 
int readRadiationIrq(void)
{    // new value from rising edges?
    if( (IC7CON1bits.ICBNE == 1)  || (IC7CON1bits.ICOV == 1) ) 
    { // Data pending- go get. Only keep latest (last in FIFO)
        while ( (IC7CON1bits.ICBNE == 1) || (IC7CON1bits.ICOV == 1) )  
        {   prevRadRising   =   RadRising  ;   // get last two values for delta calc
            IC7temp         =   IC7BUF  ;
            IC8temp         =   IC8BUF  ;
            RadRising       =   (IC8temp * 65536) + IC7temp  ;   // value could be previous entry or multi this entry
            RadFlag         = 1;        // no need to calc falling to rising time
        }
    }
    return (0) ;
}
int readRadiation(void)
{   extern unsigned int SNS_RadPeriod     ;

    readRadiationIrq()  ;
    
    if (RadFlag == 1)
    {   if(prevRadRising < RadRising)
        { RadPeriod = RadRising - prevRadRising  ;
        } 
        else 
        { RadPeriod = (RadRising+4294967296) - prevRadRising  ;  // add 32 bit count to lesser
        }
        SNS_RadPeriod=(((long) RadPeriod*(long)625)/(long) 10000) ;  // result in microSeconds
        RadFlag = 0 ;
        //  ic7,8               ic7,8
        // __/----------\_______/----------\_
        //   R          F
    }
    return  (SNS_RadPeriod)    ;
}


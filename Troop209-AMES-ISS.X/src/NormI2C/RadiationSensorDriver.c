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
    unsigned int    IC7temp         = 0 ;
    unsigned int    IC8temp         = 0 ;
    int             RadPtr          = 0 ;
    int             RadCurr         = 0 ; 
    int             RadPrev         = 0 ; 
    unsigned long   RadBuffer[16]   = {0} ;     // keep last 16 readings, only need last two

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
    IC8CON1bits.ICI = 0; // create masked interrupt every capture
    IC7CON1bits.ICI = 0; // create masked interrupt every capture
    IC8CON1bits.ICM = 3; // 3 - 3 every rising edge; 4 every 4; 5 every 16
    IC7CON1bits.ICM = 3; // 3- 3  every rising edge; 4 every 4; 5 every 16
    
    IPC5bits.IC7IP = 3;      // Set irq priority level
    IFS1bits.IC7IF = 0;      // clear any pending interrupt
    IEC1bits.IC7IE = 1;      // enable interrupt with a 1

    return (0);
}

    unsigned long   RadSignal       = 0 ;
    unsigned long   RadSignalprev   = 0 ;
    int             RadNoSigCnt     = 0 ; 
    int             RadStatus       = 0 ;
 
// void readRadiationIrq(void)
_ISR_   _IC7Interrupt(void)
{    // place newest time stamp into RadBuffer, set proce3ssing flag, clear interrupt
    if( (IC7CON1bits.ICBNE == 1)  || (IC7CON1bits.ICOV == 1) ) 
    { // Data pending- go get. Only keep latest (last in FIFO)
        while ( (IC7CON1bits.ICBNE == 1) || (IC7CON1bits.ICOV == 1) )  
        {   IC7temp         =   IC7BUF  ;
            IC8temp         =   IC8BUF  ;
            RadRising       =   ( (IC8temp * 65536) + IC7temp) -1  ;   // value could be previous entry or multi this entry
            RadBuffer[RadPtr]=  RadRising   ;
            RadPtr = (RadPtr +1) % 16       ;
            RadNoSigCnt     = 0 ;       // LEAR no detected signal counter
            IFS1bits.IC7IF  = 0 ;       // clear any pending interrupt
            RadFlag         = 1 ;       // calc rising to to rising time
        }
    }
}


int readRadiation(void)
{   extern unsigned long SNS_RadPeriod     ;

        //  ic7,8               ic7,8
        // __/--------------\___________/--------------\_
        //   R              F           R              F
        //  count[N1]                    count[N]
        //   <-- count[n]-count[n-1] --->  ( must allow for counter rollover)

    // readRadiationIrq()  ;
    RadStatus = 0 ; // Clear Status Flag
    if (RadFlag == 1)
    {   RadCurr= (16+RadPtr -1) % 16   ;
        RadPrev= (16+RadPtr -2) % 16   ;
        if(RadBuffer[RadPrev] < RadBuffer[RadCurr])
        { RadPeriod = RadBuffer[RadCurr] - RadBuffer[RadPrev]  ;    // No Rollover
        } 
        else 
        { RadPeriod = (RadBuffer[RadCurr]+4294967296) - RadBuffer[RadPrev]  ;  // Rollover; + 32 bit count to lesser
        }
        SNS_RadPeriod=(((long) RadPeriod*(long)625)/(long) 10000) ;  // result in microSeconds
        RadFlag = 0 ;
    }
    // RadNoSigCnt=0 ;     // clear No Signal Count
    // Let's also flag 'No signal' if no rising edge in two complete counter intervals
    IC7temp         =   IC7TMR  ;
    IC8temp         =   IC8TMR  ;
    RadSignal       =   (IC8temp * 65536) + IC7temp  ;
    if( RadSignal   < RadSignalprev)
    {   RadNoSigCnt ++ ;
        if (RadNoSigCnt > 2)
        {   RadStatus    = -1 ;
        }
    }
    RadSignalprev     = RadSignal ;
    return  (RadStatus)    ;
 }

void setOutputRp16(Boolean desiredOutputState)
{   const int RP16portbitOn03 = 0x0008;
    const int RP16portbitOf03=0xFFF7 ;
    
    // _TRISF3 = 0                 ;    /* configure port as output */
    // _RF3 = desiredOutputState   ;    /* set the output */
    RPOR8 = 0 ; // shut off motor driver usage
    if(desiredOutputState == 0)
    {   TRISF &= RP16portbitOf03    ;
        LATF  &= RP16portbitOf03    ;
    } else
    {   TRISF &= RP16portbitOf03    ;
        LATF  |= RP16portbitOn03    ;
    } 
}

int RP16PeriodmS   = 16    ; // default period is 16 mS
int RP16Iterations = 10000 ; // default loop runs 10,000 times
int RP16COunt      = 0 ;

int writeRP16Period(void)
{   int i = 0;
    while (RP16Iterations > 0) 
    {   setOutputRp16(1)    ;
        delay (4)           ;
        setOutputRp16(0)    ;        
        for (i=0; i< (RP16PeriodmS/4); i++)
        {   delay (4)   ;
            readRadiation() ;
        }
        RP16Iterations-- ;
    }
    return (0)  ;
}
       
    
/* 
 * File:   HdwrTest.c
 * Author: Norman McFarland
 *
 * Created BEFORE Nov 19, 2016, 3:10 PM
 * 
 * Revision 1 Notes
 * Jan 17, 2017
 * 1.1. Add #include PWM MotorDrivers.h
 * 1.2. Include untested function calibrateEncoder
 *
 * Rev 2 Feb 9, 2016
 * 2.1.  Major rewrite of readEncoder function based on diagnostic input signal
 * 2.2.  Add monitoirENcoder function  near bottom of file
 * 2.3.  Add calibrate Encoder function at bottom of file
 */ 
#include "system.h"
#include "nesi.h"
#include "../NormI2C/PortBitDrivers.h"
#include "../NormI2C/Encoder.h"
#include "multiMotorDrivers.h"
#include "stdio.h"
// #include "math.h"


#define ArraySizeMax  8

    int DiffLimit   = 4     ;
    int EncSignal1  = 0     ; 
    int EncFlag     = 0     ;   // set if need to process recent IRQ    
    int EncodPos    = 0     ;   // Raw Encoder Position without Offset
    int EncodPosRaw = 0     ;   // Raw Encoder Position without Offset

    long EncSumX1   = 0     ;
    long EncSumX2   = 0     ;
    long EncNum     = 0     ;
    long DifferenceValue    = 0 ;
    
    unsigned long   IC5temp             = 0     ;
    unsigned long   IC6temp             = 0     ;
    unsigned long   IC5time             = 0     ;
    unsigned long   IC6time             = 0     ;
    unsigned long   IC5Timer            = 0     ;
    unsigned long   EncOnTime           = 0     ;       // internal time value in counts
    unsigned long   EncPeriod           = 0     ;       // internal time value in counts
    unsigned long   EncPeriodPrev       = 0     ;       // internal time value in counts
    unsigned long   EncoderQueue[8]     = {0}   ;
    unsigned long   BellCurveBuffer[36] = {0}   ;
    unsigned long   ExpectedValue       = 12037 ;       // 66670 for period

    
    int BellCurveBufferFill(unsigned long value)
{   BellCurveBuffer[DiffLimit]  = value ;
    DiffLimit++ ;
    if(DiffLimit>36)
    {   DiffLimit=0    ;      
    }
    return (DiffLimit)  ;
}


int ErrorCheckLong(long ErrorVal)
{   // function to determine the magnitude of an error value
    int eOfs    = 0 ;
    if      ( ErrorVal < -131072) {eOfs=  0 ; } //  0
    else if ( ErrorVal < -65536)  {eOfs=  1 ; } //  1
    else if ( ErrorVal < -32768)  {eOfs=  2 ; } //  2
    else if ( ErrorVal < -16384)  {eOfs=  3 ; } //  3
    else if ( ErrorVal < -8192)   {eOfs=  4 ; } //  4
    else if ( ErrorVal < -4096)   {eOfs=  5 ; } //  5
    else if ( ErrorVal < -2048)   {eOfs=  6 ; } //  6
    else if ( ErrorVal < -1024)   {eOfs=  7 ; } //  7
    else if ( ErrorVal <  -512)   {eOfs=  8 ; } //  8
    else if ( ErrorVal <  -256)   {eOfs=  9 ; } //  9
    else if ( ErrorVal <  -128)   {eOfs= 10 ; } // 10
    else if ( ErrorVal <   -64)   {eOfs= 11 ; } // 11
    else if ( ErrorVal <   -32)   {eOfs= 12 ; } // 12
    else if ( ErrorVal <   -16)   {eOfs= 13 ; } // 13
    else if ( ErrorVal <    -8)   {eOfs= 14 ; } // 14
    else if ( ErrorVal <    -4)   {eOfs= 15 ; } // 15
    else if ( ErrorVal <    -2)   {eOfs= 16 ; } // 16
    else if ( ErrorVal ==    0)   {eOfs= 17 ; } // 17
    else if ( ErrorVal <     2)   {eOfs= 18 ; } // 18
    else if ( ErrorVal <     4)   {eOfs= 19 ; } // 19
    else if ( ErrorVal <     8)   {eOfs= 20 ; } // 20
    else if ( ErrorVal <    16)   {eOfs= 21 ; } // 21
    else if ( ErrorVal <    32)   {eOfs= 22 ; } // 22
    else if ( ErrorVal <    64)   {eOfs= 23 ; } // 23
    else if ( ErrorVal <   128)   {eOfs= 24 ; } // 24
    else if ( ErrorVal <   256)   {eOfs= 25 ; } // 25
    else if ( ErrorVal <   512)   {eOfs= 26 ; } // 26
    else if ( ErrorVal <  1024)   {eOfs= 27 ; } // 27
    else if ( ErrorVal <  2048)   {eOfs= 28 ; } // 28
    else if ( ErrorVal <  4096)   {eOfs= 29 ; } // 29
    else if ( ErrorVal <  8192)   {eOfs= 30 ; } // 30
    else if ( ErrorVal <  16384)  {eOfs= 31 ; } // 31
    else if ( ErrorVal <  32768)  {eOfs= 32 ; } // 32
    else if ( ErrorVal <  65536)  {eOfs= 33 ; } // 33
    else if ( ErrorVal <  131072) {eOfs= 34 ; } // 34
    else                          {eOfs= 35 ; } // 35
    return(eOfs)    ;
}
   
    void initEncoder(void) { // Flush everything from before
    IC5CON1 = 0;
    IC5CON2 = 0;
    IC6CON1 = 0;
    IC6CON2 = 0;
    //Set the input source as RP27

    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPINR9=0x1818    ;   //Select RP18 as input to both IC5 &IC6
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    IC6CON2bits.IC32 = 1; // pull two units apart for now
    IC5CON2bits.IC32 = 1;
    IC6CON1bits.ICTSEL = 7;
    IC5CON1bits.ICTSEL = 7; // clock src 5=timer 5; 7 = FCY (16MHz))
    IC6CON2bits.SYNCSEL = 0;
    IC5CON2bits.SYNCSEL = 0;
    IC6CON2bits.ICTRIG = 0;
    IC5CON2bits.ICTRIG = 0;
    IC6CON1bits.ICI = 0; // create masked interrrupt every capture
    IC5CON1bits.ICI = 0; // create masked interrrupt every capture
    IC6CON1bits.ICM = 1; // capture on every edge;
    IC5CON1bits.ICM = 1; // capture on every edge; 

    IPC9bits.IC5IP = 5;     // set IRQ level. do not disrupt other settings
    IFS2bits.IC5IF = 0;     // clear any pending interrupt
    IEC2bits.IC5IE = 1;     // enable interrupt with a 1
    
    return ;    
}


    unsigned long EncNewRead     = 0 ;
    unsigned long EncTempVal     = 0 ;
    unsigned long EncPrevRise    = 0 ; 
    unsigned long EncPrevFall    = 0 ; 
    
    unsigned long EncCntRiseG    = 0 ;
    unsigned long EncCntRiseB    = 0 ;
    unsigned long EncCntFallG    = 0 ;
    unsigned long EncCntFallB    = 0 ;
    unsigned long EncCntTotal    = 0 ;

    
_ISR_   _IC5Interrupt(void)
{   // new value from rising edges?
    while ( (IC5CON1bits.ICBNE == 1)  || (IC5CON1bits.ICOV == 1) ) 
    {   // Data pending- go get. Only keep latest (last in FIFO)
        // Readings are at least 1 count, maybe 2 counts over range- subtract 1 not 2
        // Get values, save into queue
        // note oldest readings go into queue first. newest enal variable
        EncSignal1 = _RD1  ;
        IC5temp               =   IC5BUF  ;
        IC6temp               =   IC6BUF  ;
        EncNewRead  =  ( ( (IC6temp << 16) + IC5temp ) -1 ) ; // & 0xFFFFFFFE ;  // value could be previous entry or multi this entry
        EncTempVal = EncNewRead - EncPrevRise   ;
        EncCntTotal++   ;
        
        if  (EncSignal1 == 1) 
        {   if (EncTempVal < 76000) // Unsigned. Filters FFFxxxx
            {   if (EncTempVal > 56000) 
                {   EncPeriod   = EncTempVal    ;
                    EncCntRiseG++               ;   
                }
            }
            EncPrevRise = EncNewRead    ;
        }
        if  (EncSignal1 == 0) 
        {   {   EncOnTime   = EncTempVal    ;
                EncCntFallG++   ;
            }
            EncPrevFall = EncNewRead    ;
        }
        EncFlag = 1         ;
    }
    IFS2bits.IC5IF = 0  ;     // clear any pending interrupt
} 

unsigned int EncoderFilter[16]  = {0} ; // 16 slot filter 
unsigned int EncNewPos          = 0 ; //    Newest Encoder Position/percentage of 4096
unsigned int EncLstPos          = 0 ; //    Last Encoder Position/percentage of 4096
unsigned int EncAverage         = 0 ; //    Average of Filter values
unsigned int EncAverageMin      = 0 ; //    Average minus range
unsigned int EncAverageMax      = 0 ; //    Average plus range
unsigned int EncSum             = 0 ; //    SUm of Filter values
unsigned int Enc                = 0 ; //    Loop counter
unsigned int EncPtr             = 0 ; //    Filter Table pointer- next space to use 
         int EncQual            = -15 ; //  QUality level +1 for good reading; -1 for out;ier reading

int filterEncoder(void)
{   EncNewPos = (EncOnTime << 12) / EncPeriod ;
if (EncNewPos > 0)
    {   // Getting valid readings; go to work
        if(EncQual < 8)
        // Need to bulk update the filter table
        {   EncoderFilter[EncPtr] = EncNewPos ;     // need to reload filter
            EncPtr = (16+EncPtr+1) % 16  ;         ;   // 
            if( EncQual <16) {EncQual++  ; }        // Increase QUality level to 15 max
        }
        else
        {   // Have enough "good" readings; calculate if valisd reading
            if ( (EncNewPos > EncAverageMin) && 
                 (EncNewPos < EncAverageMax) )
            {   EncoderFilter[EncPtr] = EncNewPos   ;   // save new position in filter
                EncPtr = (16+EncPtr+1) % 16  ;         ;   // 
                EncLstPos  = EncNewPos ;   // sav               
                if( EncQual <16) {EncQual++  ; }        // Increase QUality level to 15 max

            } else
            {   EncNewPos =  EncLstPos      ;          // use average instead of Outlier
                if (EncQual > -16) {EncQual-- ;  }      // 
            }
        }
        // always recalc average and limits
        EncSum = 0 ;
        for (Enc=0; Enc<16; Enc++)
        {   EncSum += EncoderFilter[Enc]    ;      
        }
        EncAverage      = EncSum/16 ;
        EncAverageMin   = EncAverage - 128    ;
        EncAverageMax   = EncAverage + 128    ;
    }
    return (EncNewPos)       ;
}

int readEncoder(void)
{   extern unsigned int SNS_EncodPos      ;   // Current encoder position 
    extern unsigned int SNS_EncodPosPrev  ;   // Previous Encoder position    extern unsigned int SNS_EncodOffset   ;   // when motor commanded to 180 deg, where encoder, calc 0 offset
    extern          int SNS_EncodOffset   ;   // when motor commanded to 180 deg, where encoder, calc 0 offset
    extern unsigned int SNS_EncDirFlag    ;   // 0 not set; +1 Encoder moves CW; -1 Encoder moves CCW
    
    if (EncPeriod > 56000)
    {   // EncodPos = (EncOnTime << 12) / EncPeriod ;
        EncodPosRaw = filterEncoder()  ;
    }
    if (SNS_EncDirFlag == -1)
    {// calc reverse value of on time
        EncodPos = ( (4096 +(4096 -EncodPosRaw) ) % 4096 ) ;
    }
    else
    {   EncodPos = EncodPosRaw  ;        
    }
    SNS_EncodPosPrev =  SNS_EncodPos ;   // Previous Encoder position        
    if      ( (SNS_EncodOffset <  0) && (SNS_EncDirFlag == -1) )
    {   SNS_EncodPos = (4096 + EncodPos + SNS_EncodOffset) % 4096 ;
    }
    else if ( (SNS_EncodOffset >=  0) && (SNS_EncDirFlag == -1) )
    {   SNS_EncodPos = (4096 + EncodPos - SNS_EncodOffset) % 4096 ;
    }
    else if ( (SNS_EncodOffset <  0) && (SNS_EncDirFlag == +1) )
    {   SNS_EncodPos = (4096 + EncodPos + SNS_EncodOffset) % 4096 ;
    }
    else if ( (SNS_EncodOffset >= 0) && (SNS_EncDirFlag == +1) )
    {   SNS_EncodPos = (4096 + EncodPos - SNS_EncodOffset) % 4096 ;
    }
    else if ( (SNS_EncodOffset <  0) && (SNS_EncDirFlag == 0) )
    {   SNS_EncodPos =          EncodPos  ; // no compensation yet
    }
    else if ( (SNS_EncodOffset >= 0) && (SNS_EncDirFlag == 0) )
    {   SNS_EncodPos =          EncodPos  ; // no compensation yet
    }
    if (EncFlag == 1)
    {   EncFlag =  0  ;
        // BellCurveBuffer[ (EncodPos/128) ]++ ;
        // EncSumX1 = EncSumX1 + DifferenceValue ;
        // EncSumX2 = EncSumX2 +(DifferenceValue*DifferenceValue)     ;
        // EncNum++ ;
        return (1)  ;
    }
    else
    {   return (0)   ;
    }
}

    int EncmSdelay  = 0    ; // Convert passed mS to Period counts
    unsigned long EncNextRead = 0 ;
    int calibrationError      = 0 ;
    
    int tempEncodval    = 0;
    int tempEncodErr    = 0;
    int calEncStat      = 0;
    int EncCalState     = 0 ;

    
int monitorEncoder(int attempts)    
{   // get 'attempts' new valid reads of encoder
    int attemptCount  = attempts   ;
    int EncStatus = 0 ;
    int EncRtn  = 0 ;
    
    while (attemptCount > 0)
    {   EncStatus = readEncoder() ;   // returns 0 no change; =1 change -1 no signal
        if (EncStatus == 1)
        {   attemptCount--  ;
        }
        else if (EncStatus <= -1)  
        {   EncRtn = -1        ;
            return (EncRtn)    ;
        }
    }
    EncRtn = +1      ;
    return (EncRtn)  ; // +1 returned from read Encoder setting
}    


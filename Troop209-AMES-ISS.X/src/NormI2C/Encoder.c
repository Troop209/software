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
// #include "math.h"


#define ArraySizeMax  8

    int EncTimer    = 0    ; // Number counts for while loop


        // EDT unsigned long   prevRisingArray[ArraySizeMax] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;
        // EDT unsigned long   prevFallingArray[ArraySizeMax] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;

void initEncoder(void)
{   
    // configure pin for Encoder input     
    // _PCFG5  = 1;  // PWM 12 is digital and NOT Analog input           
    _TRISD1 = 1;  // Encoder pin is an input
    _LATD1  = 0;  // drive output low

      // set the RPOR register so OC9 output goes to pin
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPINR9=0x1818    ;   //Select RP18 as input to both IC5 &IC6
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers
   
// set up encoder    
    // Clear contents of IC5 & IC6
    IC5CON1 = 0;        // disable for now. enable below
    IC5CON2 = 0;    
    
    IC6CON1 = 0;        // disable for now. enable below
    IC6CON2 = 0;   
  
    // Clear configure IC5 & IC6
    IC5CON1 = 0x1C00 ;        // measure period of Encoder PWM input
//   IC5CON2 = 0x0000 ;    
//    IC5CON2 = 0x000F ;        // Sync with timer 5  
    IC5CON2 = 0x0000 ;        // Sync (no trigger) with no other source  
    
    IC6CON1 = 0X1C00 ;        // measure duty cycle of Encoder PWM input
    IC6CON2 = 0x0000 ;        // measures the "On" time
   
    // turn on modules
    IC5CON1 |= 0x03 ;   // Enable Rising edge sensing
    
    IC6CON1 |= 0x02 ;   // Enable falling edge sensing
}
         
   
    int flag    = 0 ;     
    int tdif    = 0 ;       // time difference between IC5TMR and IC6TMR. typically 0
    int i       = 0 ;       // local loop counter
    int j       = 0 ;       // local loop counter

    //            .............|'''''''''''''''''|...............|'''''''''''''|................
    // Rising to Rising time   |                                 |
    // Falling to Falling time                   |                             |
    // On Times                |-----------------|               |-------------|
    // rise times              r                                 r
    // fall times                                f                             f
    
        unsigned long   Rising          = 0 ;       // static variables !!!
        unsigned long   prevRising      = 0 ;
        unsigned long   Falling         = 0 ;
        unsigned long   prevFalling     = 0 ;
        unsigned long   period          = 0 ;       // internal time value in counts
        unsigned long   fallperiod      = 0 ;       // internal time value in counts
        unsigned long   position        = 0 ;       // internal time value in counts
        unsigned long   EncPosition     = 0 ;
        unsigned long   EncPosition2    = 0 ;
        
        unsigned long   RECountEntry    = 0 ;   
        unsigned long   RECountIC5      = 0 ;   
        unsigned long   RECountIC6      = 0 ;   
        unsigned long   RECountFlag     = 0 ;
        
        long   scaledPosition = 0 ;
        long   scaledPeriod   = 0 ;
        long   scaledResult   = 0 ;
        

        int    ENC_OvflCnt     = 0 ; 
        
        unsigned long   prevRisingArray[ArraySizeMax]  = {0,0,0,0,0,0,0,0} ;
        unsigned long   prevFallingArray[ArraySizeMax] = {0,0,0,0,0,0,0,0} ;

        unsigned long divisor = 0 ;
    unsigned int  quotient = 0 ;
    unsigned int  multiplier =16384 ;
    unsigned int  loopvar = 0 ;
       
int readEncoder(void)
{   // Return Carousel position as 0.0 to 359.9 degrees as 0-4095 counts
    // OrReturn -1 if not detecting any signal
    extern unsigned int SNS_EncPeriod     ; // defined in SensorDrivers.c
    extern unsigned int SNS_EncodPos      ; // defined in SensorDrivers.c
    extern unsigned int SNS_EncodPosPrev  ; // defined in SensorDrivers.c
    extern unsigned int SNS_MovFlg        ; // defined in SensorDrivers.c
    extern unsigned int SNS_EncodOffset   ; // defined in SensorDrivers.c
    extern unsigned int SNS_EncDirFlag    ; // defined in SensorDrivers.c 
 
    // new value from rising edges?
    // PIC24 will queue up to 4, returned oldest to9 newest
    // get most recent set
    // (if only one, use the last one from last read)
    RECountEntry++  ;
    
    // check for incoming signal; return -1 if not present
    if ( (RECountEntry > 500) && (RECountIC5 == 0) )
        {   // Lots of entering function, but no rising edge detection
            // NO SIGNAL from peripherals
            SNS_EncodPos = -1  ;
            return (SNS_EncodPos)    ;        // Flag No Signal on incoming port
        }
    // new value from rising edges?
    // PIC24 will queue up to 4, returned oldest to newest
    // get most recent set
    // (if only one, use the last one from last read)
    // Must check que at least every 4 entries * 4 mS or every 16 ms or will get $X or 5X readings
    
    if( (IC5CON1bits.ICBNE == 1) || (IC5CON1bits.ICOV == 1) )
    { // Data pending- go get. Only keep latest (last in FIFO)
        RECountIC5++ ;
        while ( (IC5CON1bits.ICBNE == 1) || (IC5CON1bits.ICOV == 1) ) 
        {   for (i=ArraySizeMax-2; i>=0; i--)
            {   j=i+1   ;
                prevRisingArray[j]   =prevRisingArray[i]  ; // 
            }
            prevRisingArray[0]   =IC5BUF  ;
            prevRising  =   prevRisingArray[1]  ;   // get last two values for delta calc
            Rising      =   prevRisingArray[0]  ;   // value could be previous entry or multi this entry
            ENC_OvflCnt = 0 ;           // got signal, clear count
            flag        = 0 ;           // will calc with falling to rising time
            
            if(prevRisingArray[7] != 0)
            {   // don't calc if buffer not yet filled
                // period is count of 16M clock cycles
                if(prevRising > Rising)
                { period = ( (Rising - prevRising)+ 65536 ) ; // % 65536 ;  // include counter rollover
                } 
                else 
                { period = (Rising - prevRising) %65536  ;  // no counter rollover
                }
            }
            SNS_EncPeriod=period ;
        }
    }

    // new value for falling edges?
    // PIC24 will queue up to 4, returned oldest to9 newest
    // get most recent set
    // (if only one, use the last one from last read)
    if( (IC6CON1bits.ICBNE == 1) || (IC6CON1bits.ICOV == 1) )
    {   RECountIC6++ ;
        while ( (IC6CON1bits.ICBNE == 1) || (IC6CON1bits.ICOV == 1) )
        {   for (i=ArraySizeMax-2; i>=0; i--)
            {   j=i+1   ;
                prevFallingArray[j]   =prevFallingArray[i]  ; // 
            }
            prevFallingArray[0]   =IC6BUF  ;
            prevFalling =   prevFallingArray[1] ;
            Falling     =   prevFallingArray[0]  ;
            tdif        =   IC5TMR - IC6TMR    ;    // find error between IC5 and IC6
            EncTimer++     ;       // increment external time count value (~4mS per count)
            
            if(prevFallingArray[7] != 0)
            {   // don't calc if buffer not yet filled
                // result is count of 16M clock cycles
                if(prevFalling < Falling)
                { fallperiod = (Falling - prevFalling) + 65536  ; // include counter rollover
                } 
                else 
                { fallperiod = Falling - prevFalling  ;   // no counter rollover
                }
                flag        = 1 ;       // need to calc values
            }
        }
    }
    if (flag == 1)
    {   RECountFlag++   ;
        // Calc the On Time of the signal (previous rising edge to current falling edge)
        //  ic5       ic6
        // __/----------\_____
        //   R          F
        if(Falling  > Rising)
        { // default condition Rising < Falling
            position = (Falling -Rising + tdif) % 65536  ;
        }   
        else 
        { // counter rolled over, add 65536 onto value
            position = ( (Falling -Rising) + tdif + 65536 )  % 65536 ;
        }
        
        // Now calc the PerCent on time
        if( (position < period) && (period > 0) )
        {   EncPosition = position *4096  ;
//            EncPosition2= nmDivide(EncPosition, period) ;
              EncPosition2=EncPosition/period;
            SNS_EncodPos= EncPosition2 % 4096    ;
// UDT            SNS_EncodPos     = (EncPosition )  ;    // might need a mutiply factor here
        }

        // reference position to the carousel offset

        // There are 4096 "counts" in a full revoluytion of carousel
        // result is POSITIVE number, so always add 4096 to result to eliminate negative values
        // add or subtract the Encoder Offset depending on it's sign to EncodPos
        // result is POSITIVE number, so always add 4096 to result to eliminate negative values (first 4096+)
        // after all the additions limit result to range of 0-4095
        //   this done by dividing result by 4096 and returning the remainder of the division ('%'  math function)

        // SNS_EncDirFlag should be +1, or -1. ANything else means re-calibrate the encoder
        if      ( (SNS_EncodOffset <  0) && (SNS_EncDirFlag = +1) )
        {   SNS_EncodPos = (4096 + SNS_EncodPos + SNS_EncodOffset) % 4096 ;
        }
        else if ( (SNS_EncodOffset >= 0) && (SNS_EncDirFlag = +1) )
        {   SNS_EncodPos = (4096 + SNS_EncodPos - SNS_EncodOffset) % 4096 ;
        }
        else if ( (SNS_EncodOffset <  0) && (SNS_EncDirFlag = -1) )
        {   SNS_EncodPos = (4096 + SNS_EncodPos + SNS_EncodOffset) % 4096 ;
        }
        else if ( (SNS_EncodOffset <  0) && (SNS_EncDirFlag = -1) )
        {   SNS_EncodPos = (4096 + SNS_EncodPos - SNS_EncodOffset) % 4096 ;
            SNS_EncodPos = (4096 - SNS_EncodPos) % 4096 ;
        }
        else 
        {   calibrateEncoder()  ;   // destroys current motor position
        }
            
            // Is Carousel Moving???
        if (SNS_EncodPosPrev ==  SNS_EncodPos )
        { // not moving
            SNS_MovFlg  =   0 ;
        }
        else
        { //  moving
            SNS_MovFlg  =   1 ;
        }
    }
    return  (SNS_EncodPos)    ;
}

    int EncmSdelay  = 0    ; // Convert passed mS to Period counts
    int EncStat     = 0    ; // Encoder return code
    

int monitorEncoder(int mSdelay)
{   // watch encoder for specified number of millSeconds during/after motor move
    // uses frequency (period) of encoder signal to measure time    
    // Encoder period is ~ 4.0 mS so incoming value divided by 4 (ms)
    // Time value incremented in readEncoder IC6 section
    // At 4 mS per pulse, 250 counts is ~1 Second
    extern unsigned int SNS_EncodPos    ;
    
    EncmSdelay  = mSdelay/4     ; // Convert passed mS to Period counts
    EncTimer    = 0             ; // zero the loop counter      
    while (EncTimer < EncmSdelay)
    {
        EncStat=readEncoder()   ;
        nop();
        nop();
        nop();
        delay(1000); //experiment on delays that could introduce a problem. delayed up to 1 second with no problem. 
//debug file write        
//    char datatest2[80] = {0};    extern int SNS_EncodPos;
//    extern int SNS_MotorPos;
//    extern int moveStatus;
//    sprintf(datatest2,"Encoder read: Date-Time: ,%s,SNS_EncodPos,%i, SNS_MotorPos,%i,", dateTime.getStamp(), SNS_EncodPos, SNS_MotorPos);
//    dataLog.add(datatest2, 0x1010);


        if (EncStat == -1)
        {   // No SIgnal. Exit
            delay(mSdelay)  ;
            return (-1) ;
        }
    }
    return (SNS_EncodPos)  ;
 }

int calibrateEncoder(void)
{   // send motor to Calibrate Position
    // read encoder
    extern unsigned int SNS_EncodPos      ; // defined in SensorDrivers.c 
    extern unsigned int SNS_EncodOffset   ; // defined in SensorDrivers.c 
    extern unsigned int SNS_EncDirFlag    ; // defined in SensorDrivers.c 
 
    int tempEncodval    =0  ;
    int EncStat = 0      ;  // capture readEncoder Status
 
    SNS_EncodOffset= 0  ;   // clear out any previous value 
    // Send Motor/carousel to servo motor 180 degree position  
// NRM    writeMotorPositionAbs(2048) ; // send motor to 180 degrees

    EncStat=monitorEncoder(15000)    ;  // monitor encoder
    
    if (EncStat == -1)
    {   // No ENcoder Signal. Return error
        return (-1) ;
    }
    tempEncodval=SNS_EncodPos-2048       ; // keep signed offset value (ideally 0)

    // Send Motor/carousel to servo motor 90 degree position  
// NRM    writeMotorPositionAbs(1024) ; // send motor to 90 degrees

    EncStat=monitorEncoder(15000)    ;  // monitor encoder
    if (EncStat == -1)
    {   // No ENcoder Signal. Return error
        return (-1) ;
    }
    // Have signed encoder offset for 180 deg motor position in tempEncodval
    // Have encoder value for 90 deg in SNS_EncodPos
    // Calculate the value to put into SNS_EncodOffset
    SNS_EncDirFlag = 0    ; // Clear previous value (if any)
    
    if( (SNS_EncodPos > 1000) && (SNS_EncodPos < 1048) ) // set wide tolerance around 1024 count
    {   // Encoder increasing counts are same as motor counts
        SNS_EncDirFlag = +1    ;
    }
    else if ( (SNS_EncodPos > 3048) && (SNS_EncodPos < 3096) ) // set wide tolerance around 3072 count
    {   // Encoder increasing counts are opposite of motor counts
        SNS_EncDirFlag = -1    ;
    }
    else    
    { SNS_EncDirFlag = 0    ;
    }
    // Can now set the SNS_EncodOffset
    SNS_EncodOffset = tempEncodval  ;
    
    // Let's test it all and see if it works:
    //  NRM // DO MATH HERE!
    
    // NRM //SNS_EncodOffset=            ; // encoder value should be stable. capture
    // NRM //SNS_EncodOffset=SNS_EncodOffset+2048 ;  // add 180 deg offset to value 
    // NRM //SNS_EncodOffset %= 4096              ;

// NRM    writeMotorPositionAbs(3072) ; // send motor to 270 degrees
    // Wait for motor to stop, get valid encoder reading
    EncStat=monitorEncoder(15000)    ;  // monitor encoder
    if (EncStat == -1)
    {   // No ENcoder Signal. Return error
        return (-1) ;
    }

    // is motor and encoder in sync?
    if ( (SNS_EncodPos < 3048) && (SNS_EncodPos > 3096) ) // set wide tolerance around 3072 count
    {   // Calibration Failed
        return (-2)    ;
    }
    return (SNS_EncodOffset)    ;
}


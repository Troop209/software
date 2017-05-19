#include "system.h"
#include "nesi.h"
#include "PortBitDrivers.h"
#include "Encoder.h"

#define   functionSUCCESS   0
#define   functionBUSY      1
#define   functionFAULT     2

// Output COmpare StaTES

#define    RISE     0
#define    HIGH     1
#define    FALL     2
#define    LOW      3

#define    fPATTERN    -1
#define    fANGLE      -2
#define    fSPEED      -3
#define    INITIALIZE   0          
#define    IDLE         1
#define    CALC         2
#define    COMMAND      3
#define    SEND         4
#define    CHECK        5
#define    STOP         6
#define    VERIFY       7
#define    COMPLETE     8
#define    STALL        9
#define    FAULT       10
// Half steps or Full Steps for motor move

#define   HALFSTEP      0
#define   FULLSTEP      1

// Position Pattern Functions
#define RESET      -1
#define NOOPR       0
#define ABSOLUTE    1
#define RELATIVE    2
#define STATION     3
#define NEG2POS     4
#define CALIBRATE   5
#define END2END     6

int moveStatus      =  1 ;
    
const int FSFullStep = 2048 ;
const int FSHalfStep = 4096 ; 
const int FSEncoder  = 4096 ;

   int EncoderErrorLog[30] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  ;

    int carouselAngle       = 0   ;   // destination Position of Carousel
    int motorStepsPerSec    = HALFSTEP  ;
    int duration            = 0 ;   // default to 0 value (calculated below)
    int motLookUp           = 1 ;   // loop variable for motor position
    

    int motorStepsPerSec     ;

    int SNS_virtEncoderBgn   ;  
    int SNS_virtEncoderEnd   ;
    int SNS_virtEncoderNow   ;
    
     // State Machine Diagnostic Counters
    unsigned int oc8high  = 0    ;
    unsigned int oc8low   = 0    ;
    unsigned int oc8rise  = 0    ;
    unsigned int oc8fall  = 0    ;
    unsigned int oc8stat  = 0    ;
    unsigned int oc8cnt   = 0    ;
    int OC8State          = OFF  ;

    unsigned int oc9high  = 0    ;
    unsigned int oc9low   = 0    ;
    unsigned int oc9rise  = 0    ;
    unsigned int oc9fall  = 0    ;
    unsigned int oc9stat  = 0    ;
    unsigned int oc9cnt   = 0    ; 
    int OC9State          = OFF  ;

    char oc8Buffer[48]    = {0}  ;   
    char oc9Buffer[48]    = {0}  ; 
    
void initStepper(void)
{   setOnStepDisable()  ;
    /* disable module before configuring */
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR8=35  ;   //Select oc 9  as RP16
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    T5CON       = 0 ;   // clear everything out
    T5CON       = 0x0030 ;  // TImer 5 enable, /256, interrnal 16G clock
    nop()                ;  // let hardcware catch up to software (1 instruiction cycle))
    PR5         =   3125 ;  // 20 Hz POeriod  as 16000000/256/3124=20.00000 // /256 is prescaler  /3125 is compare value
    nop()                ;  // and 1 extra 
    // T5CON       = 0x8030 ;  // TImer 5 enable, /256, interrnal 16G clock
    // nop()                ;  // let hardcware catch up to software (1 instruiction cycle))
    OC9CON1 &= 0xfff8;

    /* OC config */
    OC9CON1 = 0x1C08;     // clock source Fcy, trigger mode 1, disabled
    // OC9CON1 = 0x0C08;        // clock source Tmr5, trigger mode 1, disabled
    OC9CON2 = 0x008F ;        // triggered, non inverted signal (active high)
    PR5     =  3125  ;        // set period in Timer 5
    OC9RS   =  4000  ;        // set to 250uS 'On' Time
    OC9R    =     1  ;        // ALWAYS set to 1 (begin pulse when counter ==1) 
    
    /* configure pins */
    _TRISF3 = 0;  // PWM 1 is an output
    _LATF3  = 0;  // drive output low
    _ODF3  = 1;    // Maker output and Open Drain output for 5.5v signals 3.3v will work

    /* turn module on */
    OC9CON1 |= 0x5 ;
    // NRM // Leave T5 Off: T5CON    = 0x8030 ;  // TImer 5 enable, /256, internal 16M clock

   // NRM Previous Init route split
   __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPINR3=16    ;   //Select RP16 to drive TMR2 input
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    T2CON       = 0 ;   // clear everything out
    T2CON       = 0x8002 ;  // TImer 5 enable, /256, interrnal 16G clock
    PR2         =   3125 ;  // 20 Hz POeriod  as 16000000/256/3124=20.00000 // /256 is prescaler  /3125 is compare value

    /* disable module before configuring */
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR9=25    ;   //Select #25 (OC8) to drive RP18
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    OC8CON1 &= 0xfff8;      // STop OC8
    /* OC config */
    OC8CON1 = 0x000A;       // clock source T2, clear by HW or SW 0, single shot 

    OC8CON2 = 0x009F;       // Trigger mode Sync trigStatus This Module

    OC8RS   =   4  ;    // set to no output (duty cycle = 0)
    OC8R    =   1  ;        // ALWAYS set to 1 (begin pulse when counter ==1) 
    
    /* configure pins */
    _PCFG5  = 1;  // Pin is NOT an A/D pin
    _TRISB5 = 0;  // PWM 1 is an output
    _LATB5  = 0;  // drive output high
    _RB5    = 0;  // Also set port  output
    _ODB5   = 1;    // Maker output and Open Drain output for 5v signals 3.3v will work

    /* turn module on */
    OC8CON1 |= 0x2 ;
}
void setOnStepDisable(void)
{
    setOutputDiag(1); /* set the output to be open */
}

void setOffStepDisable(void)
{
    setOutputDiag(0); /* set the output to be closed */
}
void setOnStepDirection(void)
{
    setOutputMotorEn(1); /* set the output to be open */
}

void setOffStepDirection(void)
{
    setOutputMotorEn(0); /* set the output to be closed */
}
void setOnHalfStep(void)
{
    setOutputDiag(1); /* set the output to be open */
}

void setOffHalfStep(void)
{
    setOutputDiag(0); /* set the output to be closed */
}

int ErrorCheck(int ErrorVal)
{   // function to determine the magnitude of an error value
    int eOfs    = 0 ;
    if      ( ErrorVal < -4096)   {eOfs=-12 ; }
    else if ( ErrorVal < -2048)   {eOfs=-11 ; }
    else if ( ErrorVal < -1024)   {eOfs=-10 ; }
    else if ( ErrorVal <  -512)   {eOfs=-9  ; }
    else if ( ErrorVal <  -256)   {eOfs=-8  ; }
    else if ( ErrorVal <  -128)   {eOfs=-7  ; }
    else if ( ErrorVal <   -64)   {eOfs=-6  ; }
    else if ( ErrorVal <   -32)   {eOfs=-5  ; }
    else if ( ErrorVal <   -16)   {eOfs=-4  ; }
    else if ( ErrorVal <    -8)   {eOfs=-3  ; }
    else if ( ErrorVal <    -4)   {eOfs=-2  ; }
    else if ( ErrorVal <    -2)   {eOfs=-1  ; }
    else if ( ErrorVal ==   -0)   {eOfs= 0  ; }
    else if ( ErrorVal <     2)   {eOfs= 1  ; }
    else if ( ErrorVal <     4)   {eOfs= 2  ; }
    else if ( ErrorVal <     8)   {eOfs= 3  ; }
    else if ( ErrorVal <    16)   {eOfs= 4  ; }
    else if ( ErrorVal <    32)   {eOfs= 5  ; }
    else if ( ErrorVal <    64)   {eOfs= 6  ; }
    else if ( ErrorVal <   128)   {eOfs= 7  ; }
    else if ( ErrorVal <   256)   {eOfs= 8  ; }
    else if ( ErrorVal <   512)   {eOfs= 9  ; }
    else if ( ErrorVal <  1024)   {eOfs=10  ; }
    else if ( ErrorVal <  2048)   {eOfs=11  ; }
    else if ( ErrorVal <  4096)   {eOfs=12  ; }
    else                         {eOfs=13  ; }
    return(eOfs)    ;
}

int positionPattern(int pattern, int angle)
{   extern int SNS_virtEncoderBgn ;   // Current commanded motor position 
    extern int moveStatus   ;

    extern int SNS_MotorPos    ;
    int index       = 0 ;   
    // 1    Absolute Angle
    // 2    Relative Angle  <1 and 0 deg is DISABLE MOTOR>
    // 3    0, 45, 90, 135, 180, 225, 270, 315, 0|4096 ;
    // 4    315, 270, 225, 180, 135,  90,  45,   0,  45,  90, 135, 180, 225, 270, 315, 360
    // 5    
    // 6    

    // Go 0 deg, 315 deg, 0 deg, 315 deg ...
    // User Inpt Value             0   1   2   3   4   5   6   7   8  
    // Motor Move Patterns         0   1   2   3   4   5   6   7   8
    const int motorPattern3[9] = { 0,  1,  2,  3,  4,  5,  6,  7,  8}   ;     
 
    // User Inpt Value              -8  -7  -6  -5  -4  -3  -2 -1   0   1   2   3   4   5   6   7   8  
    // Motor Move Patterns           0   1   2   3   4   5   6  7   8   9  10  11  12  13  14  15  16
    //                      cycle -315-270-215-180-135,-90,-45, 0,  0,  0, 45, 90,135,180,215,270,315 
    const int motorPattern4[17]  = {-8, -7, -6, -5, -4, -3, -2, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8}   ;     

    // User Inpt Value             -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3    4    5   6   7   8  
    // Motor Move Patterns          0   1   2   3   4   5   6   7   8   9   10  11  12   13  14  15  16
    // calibration sequence of    180, 90,270,  0,180, 90,270,  0,180, 90, 270,  0, 180, 90,270,  0,180 deg
    const int motorPattern5[17] = { 4,  2,  6,  0,  4,  2,  6,  0,  4,  2,  6,  0,  4,  2,  6,  0,  4}   ;     

    // User Inpt Value             -8  -7  -6  -5  -4  -3  -2  -1   0   1   2   3   4   5   6   7   8  
    // Motor Move Patterns          0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16
    const int motorPattern6[17] = { 6, -1,  6, -1,  6, -1,  6, -1,  6, -1,  6, -1,  6, -1,  6, -1,  6}   ;     

    // motor position in HALF STEP counts (0-4095 as 0 to 360 degrees)
    // User 'Call' offset                -8    -7    -6    -5    -4    -3    -2   -1 
    // Table offset                       0     1     2     3     4     5     6    7 
    const int carouselPosition[17] = {-4096,-3584,-3072,-2560,-2048,-1536,-1024,-512,
                                   //     0   1    2    3    4    5    6    7    8
                                   //     8   9   10   11   12   13   14   15   16         
                                          0,512,1024,1536,2048,2560,3072,3584,4096}    ;
 
    if      (pattern == -1)
    {  // treat Angle as absolute Angle- As in GO TOTHIS angle
       motLookUp    = 0 ;   // reset the motor lookup to "Start over"
    }
    else if      (pattern == 1)
    {  // treat Angle as absolute Angle- As in GO TOTHIS angle
       carouselAngle = angle  ;
    }
    else if (pattern == 2)
    {   //treat Angle as a relative Angle as in GO angle Degrees forward/Backward
        // COnvert angle to relative to current position
        SNS_MotorPos  =  carouselPosition[index+8] ;
        carouselAngle = (angle -SNS_virtEncoderBgn) % FSHalfStep    ;
    }
     // convert position pattern to position count (0-4095 as 0-360 degree)
    else if (pattern == 3)
    {   // pattern index range from 0 to 8. Each position Clockwise
        index        = motorPattern3[motLookUp] ;   // get index into position table
        SNS_MotorPos  =  carouselPosition[index+8] ;
        carouselAngle = (carouselPosition[index+8] - SNS_virtEncoderBgn) % FSHalfStep  ;   // use index to get position
        motLookUp++       ;     // count up from -8 to +8
        if (motLookUp >= +8)
        {   motLookUp = 0 ;
        }
    }
    else if (pattern == 4)
    {   // pattern index range from -8 to 8. CALIBRATION sEQUENCE 
        index        = motorPattern4[motLookUp+8] ;   // get index into carouselAngle table
        SNS_MotorPos  =  carouselPosition[index+8] ;
        carouselAngle = (carouselPosition[index+8] - SNS_virtEncoderBgn) % FSHalfStep  ;   // use index to get position
        motLookUp++       ;     // count up from -8 to +8
        if (motLookUp >= +8)
        {   motLookUp = -8 ;
        }
    }
    else if (pattern == 5)
    {   // pattern index range from -8 to 8. CALIBRATION sEQUENCE   
        index        = motorPattern5[motLookUp+8] ;   // get index into position table
        SNS_MotorPos  =  carouselPosition[index+8] ;
        carouselAngle = (carouselPosition[index+8] - SNS_virtEncoderBgn) % FSHalfStep  ;   // use index to get position
        motLookUp++       ;     // count up from -8 to +8
        if (motLookUp >= +8)
        {   motLookUp = -8 ;
        }
    }
    else if (pattern == 6)
    {   // pattern index range from -8 to 8. 0, 315, 0, 315, ...   
        index        = motorPattern6[motLookUp+8] ;   // get index into position table
        SNS_MotorPos  =  carouselPosition[index+8] ;
        carouselAngle = (carouselPosition[index+8] - SNS_virtEncoderBgn) % FSHalfStep  ;   // use index to get position
        motLookUp++       ;     // count up from -8 to +8
        if (motLookUp >= +8)
        {   motLookUp = -8 ;
        }
    }
    carouselAngle = carouselAngle % FSHalfStep    ;
    moveStatus = COMMAND  ;
    return (moveStatus)  ;
}

        float    duration32 = 0 ;    // Temp holding for duration calc
        long     durationLong   = 0 ;
int MotorStepper(int motSpeed)
{   // Perform all motor-specific calculations here
    // motSpeed is index into table of speeds (step periods/frequencies) ranging 0-7
        extern int moveStatus   ;
        extern int SNS_MotorDir ;   // Motor Direction -1 CCW; +1 CW; 0 not set

        long    CountsPerRevolutionStepper    = FSHalfStep  ; //
        
        int STEPsEQUENCE = HALFSTEP ;

              // Speed index:                 0    1   2   3   4   5   6   7   8
              // Steps per Second            25,  50, 66, 75, 87,100,125,166,250
//EDT         int MotorSpeedStepper[9]   ={1280,1240,939,826,712,620,500,376,250}  ; // Timer 5 period values: FULL Step
        const int MotorSpeedStepper[9]   ={ 640, 620,468,413,356,310,250,188,125}  ; // Timer #5 Period Values HALF STEP
        //. Calculate motor speed, duration (number of steps to move) and half/ full step move.
        if(STEPsEQUENCE == FULLSTEP)
        {   motorStepsPerSec = MotorSpeedStepper[motSpeed]*2   ; // lookup in MotorSpeedStepper *2 for Full Step Periods
            durationLong = (CountsPerRevolutionStepper * carouselAngle) ;
            durationLong = ( durationLong / 2048 )   ;   
            durationLong = durationLong % (long) 2048 ;
            duration    = (int) durationLong  ;
// DIAG            duration = carouselAngle    ;   // DIAG
            setOffHalfStep()       ;    //  move in half steps (M0=1=setOnDiag & full step  M0=0==setOffDiag)
        }
        if(STEPsEQUENCE == HALFSTEP)
        {   motorStepsPerSec = MotorSpeedStepper[motSpeed]     ; // (times 2 for half step))
            durationLong = (CountsPerRevolutionStepper * carouselAngle)    ;
            durationLong = (durationLong / 4096)    ;
            durationLong = durationLong % (long) 4096 ;
            duration    = (int) durationLong  ;
// DIAG            duration = carouselAngle    ;   // DIAG
            setOnHalfStep()       ;    //  move in half steps (M0=1=setOnDiag & full step  M0=0==setOffDiag)
        }

        // 4. Configure hardware driver board for direction and mode(half/full step) and Enable        
        if(duration<0)
        {   duration *= -1       ;  // make a positive number
            setOffStepDirection()    ;   // Set motor to turn Clockwise
            SNS_MotorDir  = -1 ;
            setOffStepDisable()        ;    // Enable Stepper Motor 
        }
        else if(duration>0)
        {   setOnStepDirection()     ;   // Set motor to turn CounterClockwise
            SNS_MotorDir  = +1 ;
            setOffStepDisable()        ;    // Enable Stepper Motor
        }
        moveStatus = SEND ;
        return (moveStatus) ;
}    

  

int checkMotor(void)    
{   extern int SNS_MotorDir ;   // Motor Direction -1 CCW; +1 CW; 0 not set
    extern int moveStatus   ;
    // Virtual Encoder Vars    
    int SNS_virtEncoderBgn= 0   ;  
    int SNS_virtEncoderEnd= 0   ;
    int SNS_virtEncoderNow= 0   ;

    int stat   = 0 ;
    int sLen =- 0 ;
    
 
    // lets check running/stopped status info
        if(OC8CON2bits.TRIGSTAT == 1)
        {   if(oc8stat==0)
            {   // TRIGSTAT just changed to 1 (or oc8stat would == 1)
                OC8State    = RISE  ;
                oc8stat     = 1 ;
                oc8rise++    ;
                oc8cnt++    ;
            }
            else
            {   OC8State    = HIGH  ;
                oc8stat     = 1 ;
                oc8high++         ;
                oc8cnt++    ;
            }
        }
        else if(OC8CON2bits.TRIGSTAT == 0)
        {   if(oc8stat==1)
            {   // TRIGSTAT just changed to 1 (or oc8stat would == 1)
                OC8State    = FALL  ;
                oc8stat=0   ;
                oc8fall++   ;
                oc8cnt++    ;
                // if (OC8TMR == 0) 
                {// Done with move... set of Step clock
                    T5CON &= 0x7FFF         ;
                    // stat = monitorEncoder(16)    ;       // NRM //
                    setOnStepDisable()    ;    // Power Down Stepper Motor
                    moveStatus = STOP           ;
                }
            }
            else
            {   OC8State    = LOW  ;
                oc8stat     = 0 ;
                oc8low++     ;
                oc8cnt++    ;
            }
        }
    // lets check running/stopped status info
        if(OC9CON2bits.TRIGSTAT == 1)
        {   if(oc9stat==0)
            {   // TRIGSTAT just changed to 1 (or oc9stat would == 1)
                OC9State    = RISE  ;
                oc9rise++           ;
                oc9cnt++    ;
            }
            else
            {   OC9State    = HIGH  ;
                oc9high++           ;
                oc9cnt++    ;
            }
        }
        else if(OC9CON2bits.TRIGSTAT == 0)
        {   if(oc9stat==1)
            {   // TRIGSTAT just changed to 1 (or oc9stat would == 1)
                OC9State    = FALL  ;
                oc9fall++   ;
                oc9stat=0   ;
                oc9cnt++    ;
            }
            else
            {   OC9State    = LOW  ;
                oc9low++    ;
                oc9cnt++    ;
           }
        }
        // Virtual Encoder Update
        if (SNS_MotorDir == +1)
        {    SNS_virtEncoderNow = (4096+SNS_virtEncoderBgn + TMR2) % FSHalfStep  ; // Using FSHalfStep since tied to stepoper count
        }
        else if (SNS_MotorDir == -1)
        {    SNS_virtEncoderNow = (4096+SNS_virtEncoderBgn - TMR2) % FSHalfStep  ; // Using FSHalfStep since tied to stepoper count
        }
        sLen = sprintf (oc8Buffer,"%i,%i,%i,%i,%i,%i,%i\n",oc8rise,oc8high,oc8fall,oc8low,oc8cnt,oc8stat,OC9State) ;
        sLen = sprintf (oc9Buffer,"%i,%i,%i,%i,%i,%i,%i\n",oc9rise,oc9high,oc9fall,oc9low,oc9cnt,oc9stat,OC9State) ;
        
        if (oc8cnt > 32000)
        { // reset counts so don't overflow
          oc8rise= 0 ;
          oc8high= 0 ;
          oc8fall= 0 ;
          oc8low = 0 ;
          oc8cnt = 0 ;  
        }
        if (oc9cnt > 32000)
        { // reset counts so don't overflow
          oc9rise= 0 ;
          oc9high= 0 ;
          oc9fall= 0 ;
          oc9low = 0 ;
          oc9cnt = 0 ;  
        }
        
        stat = readEncoder()    ;
        // Read Opto Sensor
        //readOptoSensor()  ;
        
        return (moveStatus)    ;  
}


int multiMotorMove(int AppPattern, int AppAngle, int AppSpeed)
{   extern int SNS_MotorDir         ;   // Motor Direction -1 CCW; +1 CW; 0 not set
    extern int moveStatus           ;
    extern int SNS_EncodPos       ;
    int LoopCount   = 1 ;
    int mMMstat     = 0 ;

   int EncoderError = 0 ;
   int EncoderOfs   = 0 ;

   unsigned long motorMoveRequests  = 0 ;  // big so count does not overflow 

    int pattern = 0;
    int angle   = 0 ;
    int Speed   = 6 ;

    
    // Calc Gear ratio
    // Carousel to Motor Gear Ratio * 1024
    // KGearRatio = ( (long) CarouselGearTeethCount * (long) ScaleGearRatio)/MotorGearTeethCount    ; 

  while (LoopCount > 0)         // Allow State Machine to tell self to iterate
  { if (moveStatus == INITIALIZE) 
    {   initStepper()    ;
    }
    else if      (moveStatus == IDLE) 
    {   pattern = AppPattern    ;
        angle   = AppAngle      ;
        Speed   = AppSpeed      ;
        moveStatus = CALC       ;
        
        if ( (pattern < -2) || (pattern > 6) ) 
        {    moveStatus = fPATTERN   ;   }
        if  ( (angle < -4096) || (angle > 4096) )
        {    moveStatus = fANGLE   ;   }
        if ( (Speed < 0) || (Speed > 9))
        {    moveStatus = fSPEED   ;   }
            
    }
    if (moveStatus == CALC) 
    {   moveStatus = positionPattern(pattern, angle)    ;
    }
    if (moveStatus == COMMAND) 
    {   moveStatus = MotorStepper(Speed)     ;
    }
    if (moveStatus == SEND) 
    {   // Set up virtual Encoder
        // SNS_virtEncoderBgn   WHere is move starting?
        // SNS_virtEncoderBgn   Where does move end?
        // SNS_virtEncoderBgn   Where are we now?
        if (SNS_MotorDir == +1)
        { SNS_virtEncoderEnd = (4096+ SNS_virtEncoderBgn + duration) % FSHalfStep ;
        }
        else if (SNS_MotorDir == -1)
        { SNS_virtEncoderEnd = (4096+ SNS_virtEncoderBgn - duration) % FSHalfStep ;
        }           
        SNS_virtEncoderNow = SNS_virtEncoderBgn ;
        // 5. Load PIC24 Hardware Registers with move variables
        if(duration != 0)
        {   PR5     = motorStepsPerSec  ;
            OC9RS   = 4000        ;   // set to a fixed on time of 0.250 mS (4000/16000000)
            PR2     = duration  ;   // don't use right now
            OC8RS   = duration  ;
        
            // start Step clock and trigger run
            T5CON   |= 0x8000 ;
            OC8CON2  = OC8CON2 | 0x0040  ;     // set  OC8CON2bits.OCTRIG bit
            moveStatus = CHECK ;
       }
       else
       {    // Duration (distance) of move == 0, so we DONE
            moveStatus = COMPLETE ;       
       }
    }
    if (moveStatus == CHECK) 
    {   moveStatus = checkMotor()    ;
        mMMstat = readEncoder()  ;
    }
    if (moveStatus == STOP) 
    {   // Get Encoder data
        mMMstat = monitorEncoder(14)  ; // give it 3.5 x 4mS to sample final position
        // SNS_EncodPos = SNS_virtEncoderEnd   ; // NRM TEMP till encoder runs
        // Set up for next move
        SNS_virtEncoderBgn = SNS_virtEncoderEnd  ; // Get final vEncoder Position
        EncoderError=SNS_virtEncoderBgn-SNS_EncodPos   ;\
        EncoderOfs=ErrorCheck(EncoderError)    ;
        EncoderErrorLog[EncoderOfs+12]++   ;
        
        moveStatus = VERIFY     ;
    }
    if (moveStatus == VERIFY) 
    {   // Future call to compare XEncoder to vEncoder
        // $ $ $  vEncoder may be 1/2 of xEncoder
        // Future call to verify motor went to position
        moveStatus = COMPLETE    ;
    }
    if (moveStatus == COMPLETE) 
    {   SNS_virtEncoderNow= SNS_virtEncoderEnd ; // save absolute carousel position at end of move
        motorMoveRequests++  ;       // add 1 to number of moves completed
        moveStatus = IDLE   ;       // Allow next move to issue
    } 
    LoopCount--  ;
  }
    return (moveStatus)   ;
}  

// COMMAND a Move if Motor not busy, 
// Returns SUCCESS, BUSY, or FAULT
int moveCarousel(int pattern, int angle, int speed)
{   int stat = functionFAULT    ;
    int fstat   = FAULT ;
    extern int moveStatus   ;

    // determine if we can command or if motor busy
    if ( (moveStatus  == INITIALIZE) ||
         (moveStatus  == IDLE)       )
    {   stat= multiMotorMove(pattern, angle, speed) ;
         if ( (stat == INITIALIZE) ||
              (stat == IDLE)       ) 
        {     fstat  = functionSUCCESS   ;
        }
        else if
        ( (stat == CALC) || 
          (stat == COMMAND) || 
          (stat == SEND) || 
          (stat == CHECK) || 
          (stat == STOP) || 
          (stat == VERIFY) || 
          (stat == COMPLETE) || 
          (stat == STALL) )
        {    fstat  = functionBUSY  ;
        }
        else if
        ( (stat == fPATTERN) ||
          (stat == fANGLE) || 
          (stat == fSPEED) ) 
        { fstat  = functionFAULT ;   
        }
        else 
        {   fstat  = functionFAULT ;   
        }
    }
    return (fstat)   ;    
}

// TEST if Motor  busy, 
// Returns SUCCESS (not busy), BUSY, or FAULT
int checkCarousel(int pattern, int angle, int speed)
{   extern int moveStatus   ;
    int stat    = CHECK ;
    int fstat   = FAULT ;
    

    // DO we need to check new status?
    if  (moveStatus == CHECK) 
    {    stat= multiMotorMove(pattern, angle, speed) ;   
    }
    else 
    {   stat = moveStatus   ;
        if 
        ( (stat == INITIALIZE) ||
          (stat == IDLE)       ) 
        { fstat  = functionSUCCESS   ;  // inform application that we successfully done
        }
        else if
        ( (stat == CALC) || 
          (stat == COMMAND) || 
          (stat == SEND) || 
          (stat == CHECK) || 
          (stat == VERIFY) || 
          (stat == COMPLETE) || 
          (stat == STALL) )
        { fstat  = functionBUSY  ;  // inform application that we are still busy
        }
        else if
        ( (stat == fPATTERN) ||
          (stat == fANGLE) || 
          (stat == fSPEED) ) 
        { fstat  = functionFAULT ;     // inform application that we had a Fault
        }
    }
    return (fstat)   ;    
}


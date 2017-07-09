#include "system.h"
#include "nesi.h"
#include "../NormI2C/PortBitDrivers.h"
#include "../NormI2C/Encoder.h"

// Output COmpare StaTES
#define    RISE     0
#define    HIGH     1
#define    FALL     2
#define    LOW      3

// move check Carousel states:
#define   functionSUCCESS   0
#define   functionBUSY      1
#define   functionFAULT     2

// multiMotorMove states
#define    FAULT       -4       // general Fault within multiMotorMove
#define    fPATTERN    -1       // invaliud argumanet for pattern value in user call
#define    fANGLE      -2       // invalid argumanet for Angle value in user call
#define    fSPEED      -3       // invalid argumanet for speed valuein user call
#define    INITIALIZE   0       // call the initStepper function      
#define    IDLE         1       // Ready for next motor operation
#define    HOME         10      // Send motor to Mechanical HOME using Opto Sensor feedback
#define    CALC         2       // Opoeration recieved, calcualte destination
#define    COMMAND      3       // calcuation complete, determine spreed and direction      
#define    SEND         4       // send operation to PIC24 Hardware
#define    CHECK        5       // Check if motor reached destimation yet
#define    STALL        9       // (FROM chECK) motor commanded but not moving- mustr be stalled- take action
#define    STOP         6       // stop motor operations- shut down PIC24 move functions
#define    VERIFY       7       // Verify that motor reached assigned destination based on encoder and or opto responses
#define    JOG         11       // Position Error, move this short distance to get there
#define    COMPLETE     8       // Made it- do any end-of-operatioon cleranup

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

const int FSFullStep = 2048 ;
const int FSHalfStep = 4096 ; 
const int FSEncoder  = 4096 ;

    int carouselAngle       = 0 ;   // destination Position of Carousel
    int motorStepsPerSec    = 0 ;
    int duration            = 0 ;   // default to 0 value (calculated below)
    int motLookUp           = 0 ;   // loop variable for motor position  
    int moveStatus          = 1 ;
    int motComplete         = 0 ;   // Move Sequence completed =1
    int MotFlag             = 0 ;
    int MotHome             = 0 ;   // 0 = Need to HOME carousel; 1= Carousel has been Homed

    int pattern             = 0 ;   // how will motor move
    int angle               = 0 ;   // where will motro go (0-4095=0-360 deg))
    int Speed               = 6 ;   // lookup value for speed

    int RetryCount          = 5 ;
    int LoopCount           = 1 ;

    extern unsigned long BellCurveBuffer[36] ;
    
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

    // ISR int T3Now   = 0 ;
    // ISR int T3Prev  = 0 ;

// void StepperMotorIrq(void))
_ISR_ _T3Interrupt(void)
{       T5CON &= 0x7FFF     ;   // Stop Motor Pulses
        setOnStepDisable()  ;   // Disable Stepper Motor Driver
        IFS0bits.T3IF = 0   ;   // Clear T3 Interrupt
        //IEC0bits.T3IE = 0   ;   // DIsable T3 Interrupt
        TMR3 = 0 ;          ;   // reset timer 3 time value
        MotFlag  = 1        ;   // we done
}

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

    // Leave T5 Off: T5CON    = 0x8030 ;  // TImer 5 enable, /256, internal 16M clock
    OC9CON1 |= 0x5 ;    

   // NRM Previous Init route split
   __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPINR3=0x1010    ;   //Select RP16 to drive TMR3 input
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    T3CON       = 0 ;       // clear everything out
    T3CON       = 0x8002 ;  // 
    PR3         =   5 ;     // set to a very short default motor move
    
    IPC2bits.T3IP = 5; // set IRQ level. do not disrupt other settings
    IFS0bits.T3IF = 0;      // clear any pending interrupt
    IEC0bits.T3IE = 0;      // enable interrupt with a 1
    
    /* configure pins */
    _PCFG5  = 1;  // Pin is NOT an A/D pin
    _TRISB5 = 0;  // PWM 1 is an output
    _LATB5  = 0;  // drive output high
    _RB5    = 0;  // Also set port  output
    _ODB5   = 1;    // Maker output and Open Drain output for 5v signals 3.3v will work

}


int ErrorCheck(int ErrorVal)
{   // function to determine the magnitude of an error value
    int eOfs    = 0 ;
    if      ( ErrorVal < -4096)   {eOfs= 0 ; }
    else if ( ErrorVal < -2048)   {eOfs= 1 ; }
    else if ( ErrorVal < -1024)   {eOfs= 2 ; }
    else if ( ErrorVal <  -512)   {eOfs= 3 ; }
    else if ( ErrorVal <  -256)   {eOfs= 4 ; }
    else if ( ErrorVal <  -128)   {eOfs= 5 ; }
    else if ( ErrorVal <   -64)   {eOfs= 6 ; }
    else if ( ErrorVal <   -32)   {eOfs= 7 ; }
    else if ( ErrorVal <   -16)   {eOfs= 8 ; }
    else if ( ErrorVal <    -8)   {eOfs= 9 ; }
    else if ( ErrorVal <    -4)   {eOfs=10  ; }
    else if ( ErrorVal <    -2)   {eOfs=11  ; }
    else if ( ErrorVal ==   -0)   {eOfs=12  ; }
    else if ( ErrorVal <     2)   {eOfs=13  ; }
    else if ( ErrorVal <     4)   {eOfs=14  ; }
    else if ( ErrorVal <     8)   {eOfs=15  ; }
    else if ( ErrorVal <    16)   {eOfs=16  ; }
    else if ( ErrorVal <    32)   {eOfs=17  ; }
    else if ( ErrorVal <    64)   {eOfs=18  ; }
    else if ( ErrorVal <   128)   {eOfs=19  ; }
    else if ( ErrorVal <   256)   {eOfs=20  ; }
    else if ( ErrorVal <   512)   {eOfs=21  ; }
    else if ( ErrorVal <  1024)   {eOfs=22  ; }
    else if ( ErrorVal <  2048)   {eOfs=23  ; }
    else if ( ErrorVal <  4096)   {eOfs=24  ; }
    else                          {eOfs=25  ; }
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
    const int motorPattern6[17] = { 6, -1,  6, -1,  6, -1,  6, -1,  6, -1,  6, -1,  6, -1,  6, -1,  7}   ;     

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
        // SNS_MotorPos  =  carouselPosition[index+8] ;
        carouselAngle = (angle -SNS_virtEncoderBgn) % FSHalfStep    ;
    }
     // convert position pattern to position count (0-4095 as 0-360 degree)
    else if (pattern == 3)
    {   // pattern index range from 0 to 8. Each position Clockwise
        index        = motorPattern3[motLookUp] ;   // get index into position table
        SNS_MotorPos  =  carouselPosition[index+8] ;
        carouselAngle = (carouselPosition[index+8] - SNS_virtEncoderBgn) % FSHalfStep  ;   // use index to get position
        motLookUp++       ;     // count up from -8 to +8
        //delay(1000); // delay for debugging 
        if (motLookUp >= +8)
        {   motLookUp   = 0 ;
            motComplete = 1 ;    // motor has completed full revolution of positioning
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
        long    CountsPerRevolutionStepper    = 0  ; //
        
        int STEPsEQUENCE = HALFSTEP ;

              // Speed index:                 0    1   2   3   4   5   6   7   8
              // Steps per Second            25,  50, 66, 75, 87,100,125,166,250
//EDT         int MotorSpeedStepper[9]   ={1280,1240,939,826,712,620,500,376,250}  ; // Timer 5 period values: FULL Step
        const int MotorSpeedStepper[9]   ={ 640, 620,468,413,356,310,250,188,125}  ; // Timer #5 Period Values HALF STEP
        //. Calculate motor speed, duration (number of steps to move) and half/ full step move.
        if(STEPsEQUENCE == FULLSTEP)
        {   motorStepsPerSec = MotorSpeedStepper[motSpeed]*2   ; // lookup in MotorSpeedStepper *2 for Full Step Periods
            CountsPerRevolutionStepper    = FSFullStep  ;
            durationLong = (CountsPerRevolutionStepper * carouselAngle) ;
            durationLong = ( durationLong / FSFullStep )   ;   
            durationLong = durationLong % (long) FSFullStep ;
            duration    = (int) durationLong  ;
// DIAG            duration = carouselAngle    ;   // DIAG
            setOffHalfStep()       ;    //  move in half steps (M0=1=setOnDiag & full step  M0=0==setOffDiag)
        }
        if(STEPsEQUENCE == HALFSTEP)
        {   motorStepsPerSec = MotorSpeedStepper[motSpeed]     ; // (times 2 for half step))
            CountsPerRevolutionStepper    = FSHalfStep  ;
            durationLong = (CountsPerRevolutionStepper * carouselAngle)    ;
            durationLong = (durationLong / FSHalfStep)    ;
            durationLong = durationLong % (long) FSHalfStep ;
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
{       // Virtual Encoder Vars    

    extern int SNS_MotorDir ;   // Motor Direction -1 CCW; +1 CW; 0 not set
    extern int moveStatus   ;
    extern int SNS_virtEncoderBgn   ;  
       
    if (MotFlag == 1)
    {   // Motor IRQ says move done
        setOnStepDisable()    ;    // Power Down Stepper Motor
        MotFlag  = 0 ;   // we done
        moveStatus = STOP           ;
    }
    return (moveStatus)    ;  
}

   int EncoderError = 0 ;
   int EncoderOfs   = 0 ;
   
int multiMotorMove(int AppPattern, int AppAngle, int AppSpeed)
{   extern int SNS_MotorDir         ;   // Motor Direction -1 CCW; +1 CW; 0 not set
    extern int moveStatus           ;
    extern int SNS_EncodPos         ;
    extern int SNS_virtEncoderBgn   ;
    extern int SNS_virtEncoderEnd   ;
    extern int SNS_virtEncoderNow   ;
    int mMMstat     = 0 ;


   unsigned long motorMoveRequests  = 0 ;  // big so count does not overflow 

    
    // Calc Gear ratio
    // Carousel to Motor Gear Ratio * 1024
    // KGearRatio = ( (long) CarouselGearTeethCount * (long) ScaleGearRatio)/MotorGearTeethCount    ; 
  LoopCount   = 1 ;  
  while (LoopCount > 0)         // Allow State Machine to tell self to iterate
  { 
    if (moveStatus == INITIALIZE) 
    {   initStepper()    ;
    }
    if      (moveStatus == IDLE) 
    {   pattern = AppPattern    ;
        angle   = AppAngle      ;
        Speed   = AppSpeed      ;
        moveStatus = CALC       ;
        
        if ( (pattern < -2) || (pattern > 6) ) 
        {    moveStatus = fPATTERN   ;   }
        if  ( (angle < -FSEncoder) || (angle > FSEncoder) )
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
        { SNS_virtEncoderEnd = (FSEncoder+ SNS_virtEncoderBgn + duration) % FSEncoder ;
        }
        else if (SNS_MotorDir == -1)
        { SNS_virtEncoderEnd = (FSEncoder+ SNS_virtEncoderBgn - duration) % FSEncoder ;
        }           
        SNS_virtEncoderNow = SNS_virtEncoderBgn ;
        // 5. Load PIC24 Hardware Registers with move variables
        if(duration != 0)
        {   PR5     = motorStepsPerSec  ;
            OC9RS   = 4000        ;   // set to a fixed on time of 0.250 mS (4000/16000000)
            PR3     = duration  ;   // don't use right now       
            // start Step clock and trigger run
            T5CON   |= 0x8000 ;
            IFS0bits.T3IF = 0 ;   // Clear T3 Interrupt
            IEC0bits.T3IE = 1 ;         // enable the Interrupt for end-of-move
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
        
        // EncoderError = SNS_virtEncoderEnd-SNS_EncodPos   ;
        if (SNS_MotorDir == +1)
        {   EncoderError = SNS_virtEncoderEnd-SNS_EncodPos  ;
            // EncoderError = ( (FSEncoder+ SNS_virtEncoderEnd)-SNS_EncodPos) % FSEncoder ;
        }
        else if (SNS_MotorDir == -1)
        {   EncoderError = SNS_virtEncoderEnd-SNS_EncodPos  ;
            // EncoderError = ( (FSEncoder+ SNS_virtEncoderEnd)-SNS_EncodPos) % FSEncoder ;
        }           
        
        EncoderOfs=ErrorCheck(EncoderError)    ;
        BellCurveBuffer[EncoderOfs]++   ;
        moveStatus = VERIFY     ;
    }
    if (moveStatus == VERIFY) 
    {   // Future call to compare XEncoder to vEncoder
        // $ $ $  vEncoder may be 1/2 of xEncoder
        // Future call to verify motor went to position
    /*    
        if ( (EncoderError < -125 ) || (EncoderError > 125))
        {   SNS_virtEncoderEnd = ( (FSEncoder+ SNS_virtEncoderEnd)-EncoderError) % FSEncoder ;
            SNS_virtEncoderBgn = ( (FSEncoder+ SNS_virtEncoderBgn)-EncoderError) % FSEncoder ;
            SNS_virtEncoderNow = ( (FSEncoder+ SNS_virtEncoderNow)-EncoderError) % FSEncoder ;
            angle = EncoderError    ;
            pattern = 1 ;
            LoopCount++ ;
            if ( RetryCount == 0)
            {   RetryCount = 5  ;
            }
        }
        else
        {   moveStatus = COMPLETE    ;
            RetryCount = 0  ;
            angle =     AppAngle   ;
            pattern =   AppPattern ;  
        }
        if (RetryCount > 0)
        {   RetryCount-- ;
            moveStatus = CALC    ;
        }
        else 
        { // Did not make destination
            moveStatus = COMPLETE    ;
            RetryCount = 0  ;
            angle   =   AppAngle   ;
            pattern =   AppPattern ;
        }
    */
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


int getMotorStatus(void)    
{   int fstat   = functionSUCCESS ;
    if 
    (   (moveStatus == INITIALIZE) ||
        (moveStatus == IDLE)       ) 
        { fstat  = functionSUCCESS   ;  // inform application that we successfully done
        }
    else if
    (   (moveStatus == CALC) || 
        (moveStatus == COMMAND) || 
        (moveStatus == SEND) || 
        (moveStatus == CHECK) || 
        (moveStatus == VERIFY) || 
        (moveStatus == COMPLETE) || 
        (moveStatus == STALL) )
        { fstat  = functionBUSY  ;  // inform application that we are still busy
        }
    else if
    (   (moveStatus == fPATTERN) ||
        (moveStatus == fANGLE) || 
        (moveStatus == fSPEED) ) 
        { fstat  = functionFAULT ;     // inform application that we had a Fault
        }

    return (fstat)   ;    
}


// COMMAND a Move if Motor not busy, 
// Returns SUCCESS, BUSY, or FAULT
int moveCarousel(int pattern, int angle, int speed)
{   int stat    = functionFAULT    ;
    extern int moveStatus   ;

    // determine if we can command or if motor busy
    if ( (moveStatus  == INITIALIZE) ||
         (moveStatus  == IDLE)       )
    {   stat= multiMotorMove(pattern, angle, speed) ;
    }
    return (getMotorStatus() ) ;
}

    int ChkStat    = functionFAULT    ;

// TEST if Motor  busy, 
// Returns SUCCESS (not busy), BUSY, or FAULT
int checkCarousel(int pattern, int angle, int speed)
{   extern int moveStatus   ;
    

    // DO we need to check new status?
    if  (moveStatus == CHECK) 
    {    ChkStat= multiMotorMove(pattern, angle, speed) ;   
    }
    ChkStat= getMotorStatus()  ;
    return (ChkStat) ;
}

int readCarouselHome(void)
   {    extern int SNS_virtEncoderNow    ;
        extern int SNS_EncDirFlag        ;   // 0 not set; +1 Encoder moves CW; -1 Encoder moves CCW
       // function to read following values from NVM
       SNS_virtEncoderNow   = 2640   ;
       SNS_EncDirFlag       = +1    ;   // Define encoder increases angle as motor increases angle   
       return(SNS_virtEncoderNow)   ;
   }

           int encoderHome = 0    ;   // Preset encoder Hoem Angle
           int homeStat    = 0    ;
 
int homeCarousel(void)
{   //    if (moveStatus == HOME)
    extern int SNS_virtEncoderBgn ;  
    extern int SNS_virtEncoderEnd ;
    extern int SNS_virtEncoderNow ;
    extern int SNS_EncodOffset    ;   // when motor commanded to 180 deg, where encoder, calc 0 offset
    extern int SNS_EncodPos       ;   // Current encoder position
    extern int SNS_EncDirFlag     ;   // 0 not set; +1 Encoder moves CW; -1 Encoder moves CCW

    // 1. Zero the Encoder Offset
    // 2. Get the current Encoder position (Raw Position since Offset = 0)
    // 3. Get the HOME Encoder Position where carousel at 0 degrees
    // 4. Calculate angle differences
    // 5. Command Motor
    // 6. Wait for move complete
    // 7. Update encoder and virt Encoder values to home
  
    // 1. Zero the Encoder Offset
    SNS_EncodOffset = 0 ;
    // 2. Get the current Encoder position (Raw Position since Offset = 0)
    ChkStat=monitorEncoder(50) ;
    if (ChkStat == 1)
    { SNS_virtEncoderBgn = SNS_EncodPos ;        
    }
    // 3. Get the HOME Encoder Position where carousel at 0 degrees
    encoderHome = readCarouselHome()  ; // also set SNS_DirFlg
    // 4. Calculate angle differences
    // angle = ( (FSHalfStep + encoderHome - SNS_virtEncoderBgn) % FSHalfStep ) ;
    angle = encoderHome - SNS_virtEncoderBgn ;
    // if(SNS_virtEncoderBgn > encoderHome)
    // {   angle *= -1 ;   // Go in reverse direction
    // }
    pattern = 1     ;       // set to perform Absolute move from current position

    // 5. Command Motor
    homeStat = moveCarousel (pattern, angle, Speed) ;
    // 6. Wait for move complete
    while (homeStat == 1)
    {   homeStat = checkCarousel(pattern, angle, Speed) ;
    }
    // 7. Update encoder and virtual Encoder values to home
     
    SNS_virtEncoderBgn =    0   ;
    SNS_virtEncoderEnd =    0   ;
    SNS_virtEncoderNow =    0   ;
    SNS_EncodOffset    =    encoderHome ;
    SNS_EncDirFlag     =    +1 ;       
    ChkStat=monitorEncoder(25) ;            
    // MotHome still == 0
    // now set the standard default values for carousel moves
    angle = 512     ;
    pattern = 3     ;       // set to perform RELATIVE move from current position
    return (0)      ;    
}


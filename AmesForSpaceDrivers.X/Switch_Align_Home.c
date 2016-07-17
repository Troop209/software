#include "P24FJ256GB106.h" 
#include "system.h"
#include "nesi.h"
#include "Switch_Align_Home.h"
// #include "Switch_Align_Home.h"

// Read (with automatic initiialize of all bit registers) for Align switch
// Signal is Activ e lOw !!!  1== oFF; 0 == ON        
int Read_Switch_Align(void)
{   int Align   =0 ;
    AD1PCFGLbits.PCFG12 = 1     ;   // prevent use as A/D input #12
    AD1CSSLbits.CSSL12  = 0     ;   // A/D shall not scan this input
    CNPU2bits.CN30PUE   = 1     ;   // Enable internal pullup for switch
    CNPU1bits.CN15PUE   = 1     ;   // Enable internal pullup for switch
    TRISBbits.TRISB12   = 1     ;   // Set Port B bit 12 to input
    TRISDbits.TRISD6    = 1     ;   // Set Port D bit 6 to input
    nop()   ;
    nop()   ;
    Align=PORTDbits.RD6 *2 + PORTBbits.RB12  ;
    return (Align) ;
}
// Read (with automatic initialize of all bit registers) for Home switch
// Signal is Activ e lOw !!!  1== oFF; 0 == ON        
int Read_Switch_Home(void)
{   int Home   =0 ;
    // AD1PCFGLbits.PCFG12 = 1     ;   // prevent use as A/D input #12
    // AD1CSSLbits.CSSL12  = 0     ;   // A/D shall not scan this input
    CNPU4bits.CN52PUE   = 1     ;   // Enable internal pullup for switch
    CNPU4bits.CN50PUE   = 1     ;   // Enable internal pullup for switch
    TRISDbits.TRISD3    = 1     ;   // Set Port B bit 12 to input
    TRISDbits.TRISD1    = 1     ;   // Set Port D bit 6 to input
    nop()   ;
    nop()   ;
    Home=PORTDbits.RD3 *2 + PORTDbits.RD1  ;
    return (Home) ;
}

void setOutputCam1(Boolean desiredOutputState)
{
    _TRISE1 = 0                 ;    /* configure port as output */
    _RE1 = desiredOutputState   ;    /* set the output */
   
}
void setOnCam1(void)
{
    setOutputCam1(1); /* set the output to be open */
}

void setOffCam1(void)
{
    setOutputCam1(0); /* set the output to be closed */
}

void initializeCam1(void)
{
   /* no Initialization needed*/
}


/******************************************************
 *
 * Power Driver Cam2     Schematic: Control_B   PIC24 pin: 60
 *
 *****************************************************
 */


void setOutputCam2(Boolean desiredOutputState)
{
    _TRISE0 = 0;    /* configure port as output */
    _RE0 = desiredOutputState; /* set the output */

}

void setOnCam2(void)
{
    setOutputCam2(1); /* set the output to be open */
}

void setOffCam2(void)
{
    setOutputCam2(0); /* set the output to be closed */
}

void initializeCam2(void)
{
   /* no Initialization needed*/
}


/******************************************************
 *
 * Power Driver LED1     Schematic: Control_Red   PIC24 pin: 23
 *
 ******************************************************/


void setOutputLED1(Boolean desiredOutputState)
{   __builtin_write_OSCCONL(OSCCON & 0xBF)  ; // unlock Peripheral Pin Select Registers
     RPOR4=0x0000                           ;   /* clear RP8 and RP9 so Port drives pins */
    __builtin_write_OSCCONL(OSCCON | 0x40)  ; // lock Peripheral Pin Select Registers
    _TRISB9 = 0;    /* configure port as output */
    _RB9 = desiredOutputState; /* set the output */

}

void setOnLED1(void)
{
    setOutputLED1(1); /* set the output to be open */
}

void setOffLED1(void)
{
    setOutputLED1(0); /* set the output to be closed */
}

void initializeLED1(void)
{
   /* no Initialization needed*/
}

/******************************************************
 *
 * Power Driver LED2     Schematic: COntrol_Blue   PIC24 pin: 22
 *
 ******************************************************/


void setOutputLED2(Boolean desiredOutputState)
{   __builtin_write_OSCCONL(OSCCON & 0xBF)  ; // unlock Peripheral Pin Select Registers
     RPOR4=0x0000                           ; /* clear RP8 and RP9 so Port drives pins */
    __builtin_write_OSCCONL(OSCCON | 0x40)  ; // lock Peripheral Pin Select Registers
    _TRISB8 = 0;    /* configure port as output */
    _RB8 = desiredOutputState; /* set the output */

}

void setOnLED2(void)
{
    setOutputLED2(1); /* set the output to be open */
}

void setOffLED2(void)
{
    setOutputLED2(0); /* set the output to be closed */
}

void initializeLED2(void)
{
   /* no Initialization needed*/
}

/******************************************************
 *
 * Power Driver Servo     Schematic: Control_A   PIC24 pin: 3
 *
 ******************************************************/

void setOutputServo(Boolean desiredOutputState)
{
    _TRISE7 = 0;    /* configure port as output */
    _RE7 = desiredOutputState; /* set the output */

}

void setOnServo(void)
{
    setOutputServo(1); /* set the output to be open */
}

void setOffServo(void)
{
    setOutputServo(0); /* set the output to be closed */
}

void initializeServo(void)
{
    /* no initialization needed*/
}

/******************************************************
 *
 * Abstraction Layer
 *
 ******************************************************/
/*
const PowerDriver powerDriverServo = {set:setOutputServo, init:initializeServo, on:setOnServo, off:setOffServo, isOk:isOkServo},
                  powerDriverCam1  = {set:setOutputCam1,  init:initializeCam1,  on:setOnCam1,  off:setOffCam1,  isOk:isOkCam1},
                  powerDriverCam2  = {set:setOutputCam2,  init:initializeCam2,  on:setOnCam2,  off:setOffCam2,  isOk:isOkCam2},
                  powerDriverLED1  = {set:setOutputLED1,  init:initializeLED1,  on:setOnLED1,  off:setOffLED1,  isOk:isOkLED1},
                  powerDriverLED2  = {set:setOutputLED2,  init:initializeLED2,  on:setOnLED2,  off:setOffLED2,  isOk:isOkLED2};

const ServoSwitches AlignServoSwitch ={ read:Read_Switch_Align, init:Switch_init_Align},
                    HomeServoSwitch  ={ read:Read_Switch_Home,  init:Read_Switch_Align};
*/
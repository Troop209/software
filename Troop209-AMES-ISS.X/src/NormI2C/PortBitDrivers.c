/* 
 * File:   PortBitDrivers.c
 * Author: Norman McFarland
 *
 * Created on Nov 17, 2016, 3:10 PM
 * 
 * Revision Notes
 * Rev 1Jan 19, 2017
 * 1.1. Add bit drivers for Energy Harvest Enable (HarvEn) and Motor Enable (MotorEn)
 * 1.2. Add 
 * Rev 2 Feb 09 2017
 * 2.1 Change unneeded HarvEN drivers back to Daig drivers (Used to tinme functions)
 * Rev 3 Mar 11, 2017
 * 3.1 Add On Off COntrols for port RG9 as SetOnRG9/SetOffRf3
 */
#include "P24FJ256GB106.h" 
#include "system.h"
#include "nesi.h"
#include "PortBitDrivers.h"

const int portbitOn00 = 0xFFF1;
const int portbitOn01 = 0x0002;
const int portbitOn02 = 0x0004;
const int portbitOn03 = 0x0008;

const int portbitOn04 = 0x0010;
const int portbitOn05 = 0x0020;
const int portbitOn06 = 0x0040;
const int portbitOn07 = 0x0080;

const int portbitOn08 = 0x0100;
const int portbitOn09 = 0x0200;
const int portbitOn10 = 0x0400;
const int portbitOn11 = 0x0800;

const int portbitOn12 = 0x1000;
const int portbitOn13 = 0x2000;
const int portbitOn14 = 0x4000;
const int portbitOn15 = 0x8000;

const int portbitOf00=0xFFFE ;
const int portbitOf01=0xFFFD ;
const int portbitOf02=0xFFFB ;
const int portbitOf03=0xFFF7 ;

const int portbitOf04=0xFFEF ;
const int portbitOf05=0xFFDF ;
const int portbitOf06=0xFFBF ;
const int portbitOf07=0xFF7F ;

const int portbitOf08=0xFEFF ;
const int portbitOf09=0xFDFF ;
const int portbitOf10=0xFBFF ;
const int portbitOf11=0xF7FF ;

const int portbitOf12=0xEFFF ;
const int portbitOf13=0xDFFF ;
const int portbitOf14=0xBFFF ;
const int portbitOf15=0x7FFF ;

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
    // _TRISE1 = 0                 ;    /* configure port as output */
    // _RE1 = desiredOutputState   ;    /* set the output */
    
    if(desiredOutputState == 0)
    {   TRISE &= portbitOf01    ;
        LATE &= portbitOf01    ;
    } else
    {   TRISE &= portbitOf01    ;
        LATE |= portbitOn01    ;
    } 
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
    // _TRISE0 = 0;    /* configure port as output */
    // _RE0 = desiredOutputState; /* set the output */
   
    if(desiredOutputState == 0)
    {   TRISE &= portbitOf00    ;
        LATE &= portbitOf00    ;
    } else
    {   TRISE &= portbitOf00    ;
        LATE |= portbitOn00    ;
    } 

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
    //_TRISB9 = 0;    /* configure port as output */
    //_RB9 = desiredOutputState; /* set the output */
    AD1PCFGLbits.PCFG9 = 1     ;   // prevent use as A/D input #9
    if(desiredOutputState == 0)
    {   TRISB &= portbitOf09    ;
        LATB &= portbitOf09    ;
    } else
    {   TRISB &= portbitOf09    ;
        LATB |= portbitOn09    ;
    } 

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
    // _TRISB8 = 0;    /* configure port as output */
    // _RB8 = desiredOutputState; /* set the output */
    AD1PCFGLbits.PCFG8 = 1     ;   // prevent use as A/D input #8
    if(desiredOutputState == 0)
    {   TRISB &= portbitOf08    ;
        LATB &= portbitOf08    ;
    } else
    {   TRISB &= portbitOf08    ;
        LATB |= portbitOn08    ;
    } 

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
    // _TRISE7 = 0;    /* configure port as output */
    // _RE7 = desiredOutputState; /* set the output */
    if(desiredOutputState == 0)
    {   TRISE &= portbitOf07    ;
        LATE &= portbitOf07    ;
    } else
    {   TRISE &= portbitOf07    ;
        LATE |= portbitOn07    ;
    } 

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

/******************************************************************
 *
 * Motor Control CW Output Pin RB11  PIC24 pin: 24 JP8-6
 *
 *****************************************************************
 */

void setOutputMotCW(Boolean desiredOutputState)
{
    // _TRISB11 = 0                 ;    /* configure port as output */
    // _RB11 = desiredOutputState   ;    /* set the output */
     AD1PCFGLbits.PCFG11 = 1     ;   // prevent use as A/D input #11
     if(desiredOutputState == 0)
    {   TRISB &= portbitOf11    ;
        LATB &= portbitOf11    ;
    } else
    {   TRISB &= portbitOf11    ;
        LATB |= portbitOn11    ;
    } 
   
}
void setOnMotCW(void)
{
    setOutputMotCW(1); /* set the output to be open */
}

void setOffMotCW(void)
{
    setOutputMotCW(0); /* set the output to be closed */
}

void initializeMotCW(void)
{
   /* no Initialization needed*/
}

/******************************************************************
 *
 * * Motor Control CCW Output Pin RB13  PIC24 pin: 28 JP8-8
 *
 ******************************************************************
 */

void setOutputMotCCW(Boolean desiredOutputState)
{
    // _TRISB13 = 0                 ;    /* configure port as output */
    // _RB13 = desiredOutputState   ;    /* set the output */
    AD1PCFGLbits.PCFG13 = 1     ;   // prevent use as A/D input #13
   if(desiredOutputState == 0)
    {   TRISB &= portbitOf13    ;
        LATB &= portbitOf13    ;
    } else
    {   TRISB &= portbitOf13    ;
        LATB |= portbitOn13    ;
    } 
  
}
void setOnMotCCW(void)
{
    setOutputMotCCW(1); /* set the output to be open */
}

void setOffMotCCW(void)
{
    setOutputMotCCW(0); /* set the output to be closed */
}

void initializeMotCCW(void)
{
   /* no Initialization needed*/
}



void setOutputMotEna(Boolean desiredOutputState)
{
    // _TRISB5 = 0                 ;    /* configure port as output */
    // _RB5 = desiredOutputState   ;    /* set the output */
       AD1PCFGLbits.PCFG12 = 1     ;   // prevent use as A/D input #5
    if(desiredOutputState == 0)
    {   TRISB &= portbitOf12    ;
        LATB &= portbitOf12    ;
    } else
    {   TRISB &= portbitOf12    ;
        LATB |= portbitOn12    ;
    } 
}
void setOnMotEna(void)
{
    setOutputMotEna(1); /* set the output to be open */
}

void setOffMotEna(void)
{
    setOutputMotEna(0); /* set the output to be closed */
}

void initializeMotEna(void)
{
   /* no Initialization needed*/
}

void setOutput6Volt(Boolean desiredOutputState)
{
    // _TRISB5 = 0                 ;    /* configure port as output */
    // _RB5 = desiredOutputState   ;    /* set the output */
    if(desiredOutputState == 0)
    {   TRISE &= portbitOf06    ;
        LATE &= portbitOf06    ;
    } else
    {   TRISE &= portbitOf06    ;
        LATE |= portbitOn06    ;
    } 

}
void setOn6Volt(void)
{
    setOutput6Volt(1); /* set the output to be open */
}

void setOff6Volt(void)
{
    setOutput6Volt(0); /* set the output to be closed */
}

void initialize6Volt(void)
{
   /* no Initialization needed*/
}

void setOutputDiag(Boolean desiredOutputState)
{
    // _TRISB10 = 0                 ;    /* configure port as output */
    // _RB10 = desiredOutputState   ;    /* set the output */
    AD1PCFGLbits.PCFG10 = 1     ;   // prevent use as A/D input #10
    if(desiredOutputState == 0)
    {   TRISB &= portbitOf10    ;
        LATB &= portbitOf10    ;
    } else
    {   TRISB &= portbitOf10    ;
        LATB |= portbitOn10    ;
    } 
   
}
void setOnDiag(void)
{
    setOutputDiag(1); /* set the output to be open */
}

void setOffDiag(void)
{
    setOutputDiag(0); /* set the output to be closed */
}

void initializeDiag(void)
{
   /* no Initialization needed*/
}

void setOutputMotorEn(Boolean desiredOutputState)
{
    // _TRISB10 = 0                 ;    /* configure port as output */
    // _RB10 = desiredOutputState   ;    /* set the output */
    AD1PCFGLbits.PCFG12 = 1     ;   // prevent use as A/D input #10
    if(desiredOutputState == 0)
    {   TRISB &= portbitOf12    ;
        LATB &= portbitOf12    ;
    } else
    {   TRISB &= portbitOf12    ;
        LATB |= portbitOn12    ;
    } 
   
}
void setOnMotorEn(void)
{
    setOutputMotorEn(1); /* set the output to be open */
}

void setOffMotorEn(void)
{
    setOutputMotorEn(0); /* set the output to be closed */
}

void initializeMotorEn(void)
{
   /* no Initialization needed*/
}



void setOutputRG9(Boolean desiredOutputState)
{
    // _TRISB10 = 0                 ;    /* configure port as output */
    // _RB10 = desiredOutputState   ;    /* set the output */
    AD1PCFGLbits.PCFG12 = 1     ;   // prevent use as A/D input #10
    if(desiredOutputState == 0)
    {   TRISG &= portbitOf09    ;
        LATG &= portbitOf09    ;
    } else
    {   TRISG &= portbitOf09    ;
        LATG |= portbitOn09    ;
    } 
   
}
void setOnRG9(void)
{
    setOutputRG9(1); /* set the output to be open */
}

void setOffRG9(void)
{
    setOutputRG9(0); /* set the output to be closed */
}

void initializeRG9(void)
{
   /* no Initialization needed*/
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


void setUart1Tx(Boolean desiredOutputState)
{   // RP26 in RPOP13 Port RG7
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR13=0        ;   // Deselect any pre-defrined function for port
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    if(desiredOutputState == 0)
    {   TRISG &= portbitOf07    ;
        LATG  &= portbitOf07    ;
    } else
    {   TRISG &= portbitOf07    ;
        LATG  |= portbitOn07    ;
    } 
}
void setUart1Rx(Boolean desiredOutputState)
{   // RP19 in RPOP9 Port RG8
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR9=0        ;   // Deselect any pre-defrined function for port
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    if(desiredOutputState == 0)
    {   TRISG &= portbitOf08    ;
        LATG  &= portbitOf08    ;
    } else
    {   TRISG &= portbitOf08    ;
        LATG  |= portbitOn08    ;
    } 
}
void setUart2Tx(Boolean desiredOutputState)
{   // RP20 in RPOP10 Port RD5
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR10=0        ;   // Deselect any pre-defrined function for port
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    if(desiredOutputState == 0)
    {   TRISD &= portbitOf05    ;
        LATD  &= portbitOf05    ;
    } else
    {   TRISD &= portbitOf05    ;
        LATD  |= portbitOn05    ;
    }  
}
void setUart2Rx(Boolean desiredOutputState)
{   // RP25 in RPOP12 Port RD4
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR12=0        ;   // Deselect any pre-defrined function for port
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    if(desiredOutputState == 0)
    {   TRISD &= portbitOf04    ;
        LATD  &= portbitOf04    ;
    } else
    {   TRISD &= portbitOf04    ;
        LATD  |= portbitOn04    ;
    } 
}

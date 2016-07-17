/*
 * 2009 Microchip Technology Inc.
*
* FileName:        	i2c_master.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       	PIC24F
* Compiler:        	MPLAB C30 v3.11 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all 
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your 
* behalf, exclusively with Microchip's proprietary products.  Your 
* acceptance and/or use of this code constitutes agreement to the 
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT 
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS 
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH 
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE 
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, 
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR 
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER 
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP 
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author        	Date      	Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* McFarland, Norman	02/03/16	First release of source file
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code uses the PERIPHERAL LIBRARY support available with MPLAB C30 Compiler
* Code Tested on:
* EXPLORER 16 demo board with PIC24FJ256GB110 controller
* The Processor starts with the External Crystal (8 Mhz).
*
* I2C Master is configured on  I2C1 available on device
* I2C MASTER: 	I2C1 Module
*
*Connections:
*		I2C1 (MASTER)										I2Cx (SLAVE)
*														(Another Device)
*		SCL1 (pin 66)	<----------Pull up resister (4.7K) to Vcc--------------->	SCLx
*		SDA1 (pin 67)	<----------Pull up resister (4.7K) to Vcc--------------->	SDAx
*
* Pull up resister of 4.7K is necessary to tie the lines connecting SCL1 & SCLx to Vcc.
* Pull up resister of 4.7K is necessary to tie the lines connecting SDA1 & SDAx to Vcc.
*********************************************************************
 */

//#include "P24FJ256GB106.h"    // included in system.h

#include "system.h"
#include "nesi.h"
#include "PIC24_I2C2_R7.h"
/*
#include "PIC24_I2C2_R5.h"
#include "Real Time Clock.h"
#include "Switch_Align_Home.h"
#include "PIC24_Peripherals.h"
// #include "Analog2digital.c"
*/

int I2C2_Diagnostics[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0} ;
 // Made int's so they times can be adjusted with diag routinbe

int I2C2_Bit_TimeOut  = 520     ;  // Guestimated single bit time in tight while statements
int I2C2_Byte_TimeOut = 5200    ;  // (8 bits + ACK +Tolerance of 1 bit time) 
int I2C2_Bit_TimeOutStDev = 0   ;  // Standard Deviation of x count

const int SBUFLEN =   16            ;    
const int RBUFLEN =   64            ;

char    I2C2_sbuf[16]               ;   
char    I2C2_rbuf[64]               ;

 
char    I2C2_Data=  0          ;
 
char *I2C2_sbufptr  =   &I2C2_sbuf[0]    ;
char *I2C2_rbufptr  =   &I2C2_rbuf[0]    ;
int  *I2C2_diagnostic=  &I2C2_Diagnostics[0]  ;


int I2C2_Msg_Ack_cnt        =0 ; 
int I2C2_Msg_Nak_cnt        =0 ;
int I2C2_Msg_Reset_cnt      =0 ;
int I2C2_Msg_Req_cnt        =0 ;
    
int I2C2_Snd_Byte_cnt_TO2   =0 ;
int I2C2_Snd_Byte_cnt_TO3   =0 ;
int I2C2_Snd_Byte_cnt_TO4   =0 ;
int I2C2_Snd_Byte_cnt_ACK   =0 ;
int I2C2_Snd_Byte_cnt_NAK   =0 ;

int I2C2_Get_Byte_cnt_TO2   =0 ;
int I2C2_Get_Byte_cnt_TO3   =0 ;
int I2C2_Get_Byte_cnt_TO4   =0 ;
int I2C2_Get_Byte_cnt_ACK   =0 ;
int I2C2_Get_Byte_cnt_NAK   =0 ;

int I2c2_STop_Bit_Width_X1  =0 ;    // used to ddetermine typical bit width at baud 
int I2c2_STop_Bit_Width_X2  =0 ;    // used to determine typical bit width at baud 
int I2c2_STop_Bit_WidthCnt  =0 ;    // used to determine typical bit width at baud 
    
// #define I2C2_ADDR_RGB   0x39 ;      // RGB & Gesture sensor


// ****************************************************************************
//
//      bit level functions-  begin
//
// ****************************************************************************


// function 01
//function initiates I2C2 module to baud rate BRG
void i2c2_init(int KHz)
{  int temp;
   int BRG =215 ;  // $ $ $ $ $ $ $ $ 
   // BRG=(FCY/(KHz*1000))-1   ;   // Calc the Baud rate value
   I2C2BRG = BRG            ;   // Set Baud Rate Generator (BRG) Value
   TRISF   = 0x0003B        ;   // Set Data Direction (tris-state) reg to outputs
   I2C2CONbits.I2CEN = 0    ;	// Disable I2C Mode
   IFS3bits.MI2C2IF  = 0     ;	// Clear Interrupt
   I2C2CONbits.I2CEN = 1    ;	// Enable I2C Mode
   temp = I2C2RCV           ;   // read buffer to clear buffer full
   i2c2_stop();             ;   // set bus to idle- if happen s to be busy
   return   ;
}
// function 02
int i2c2_reset_bus(void) 
{ // Let's wait for STOP bit to complete (just in case)
  int x = 0;

  while (I2C2CONbits.PEN)
   { if (x > I2C2_Bit_TimeOut)
     {  return   I2C2_TIME_OUT2 ;
     }
     x++  ;
   }
  I2C2CONbits.RCEN = 0;
  IFS3bits.MI2C2IF = 0; // Clear Interrupt
  I2C2STATbits.IWCOL = 0;
  I2C2STATbits.BCL = 0;
  return I2C2_SUCCESS       ;
}

// function 03
//function initiates a start condition on bus
int i2c2_start(void)
{  int x = 0;
   I2C2CONbits.ACKDT = 0;	//Reset any previous Ack
   I2C2CONbits.SEN = 1;	//Initiate Start condition
   //the hardware will automatically clear Start Bit
   //wait for automatic clear before proceeding
   while (I2C2CONbits.SEN)
   { if (x > I2C2_Bit_TimeOut)
     {  return   I2C2_TIME_OUT2 ;
     }
     x++  ;
   }
   return I2C2_SUCCESS  ;
}
// MODIFIED on May 10
// function 04
int i2c2_restart(void)
{  int x = 0;
   // delay just a little while- let previous transaction get done
   while (((I2C2STATbits.TRSTAT) || (I2C2CONbits.RCEN)) ==1)
     { if (x > I2C2_Bit_TimeOut)
     {  return   I2C2_TIME_OUT3 ;
     }
     x++  ;
   }
  I2C2CONbits.RSEN = 1;	//Initiate restart condition
   //the hardware will automatically clear restart bit
   //wait for automatic clear before proceding
   while (I2C2CONbits.RSEN > 0)
    { if (x > I2C2_Bit_TimeOut)
     {  return   I2C2_TIME_OUT2 ;
     }
     x++  ;
   }
   return I2C2_SUCCESS   ;   ;
}

// function 05
//Resets the I2C bus to Idle
int i2c2_stop(void)
{   int x = 0;
   //initiate stop bit
   I2C2CONbits.PEN = 1;
   //wait for hardware clear of stop bit
   while (I2C2CONbits.PEN)
   { if (x > I2C2_Bit_TimeOut)
     {  return   I2C2_TIME_OUT3 ;
     }
     x++  ;
   }
   // STOP bit is determined by Baud Rate
   // Above while loop determined by clock speed and baud rate
   // Use above x value to calculate real values of bit and byte width.
   // Update field values every 30 stop bits.
   I2c2_STop_Bit_Width_X1 = I2c2_STop_Bit_Width_X1 + x ;
   I2c2_STop_Bit_Width_X2 = I2c2_STop_Bit_Width_X2 + (x * x) ;
   I2c2_STop_Bit_WidthCnt++   ;
   
   if(I2c2_STop_Bit_WidthCnt % 30)
   { // I2C2_Bit_TimeOut       = (I2c2_STop_Bit_Width_X1/I2c2_STop_Bit_WidthCnt)   ;
     // I2C2_Bit_TimeOutStDev  = sqrt((I2c2_STop_Bit_Width_X2 -(I2c2_STop_Bit_Width_X1 * I2c2_STop_Bit_Width_X1))/I2c2_STop_Bit_WidthCnt) ;
     // I2C2_Byte_TimeOut    = I2C2_Bit_TimeOut *10 ;     
   }
   I2c2_STop_Bit_Width_X1 = 0 ;
   I2c2_STop_Bit_Width_X2 = 0 ;
   I2c2_STop_Bit_WidthCnt = 0  ;
   
   return I2C2_SUCCESS      ;
}

// ****************************************************************************
//
//      bit level functions-  end
//
//      Byte level functions- begin
//
// ****************************************************************************

// function 06
// basic 7-bit I2C address send with R/W set to Read 
int i2c2_send_addr_rd (char addr)
    // I2C StandArd dictates that the Address be shifted 1 bit left and then read write 1/0 bit be placed in the LSB
{   int taddr = (int) addr    ;         // need to support extra addr width caused by shift left
    taddr=(taddr << 1)+1      ;         // +1 for READ
    return  (i2c2_send_byte_ack((char) taddr))  ;
}  

// function 07
// basic 7-bit I2C address send with R/W set to Write
int i2c2_send_addr_wt (char addr)
    // I2C StandArd dictates that the Address be shifted 1 bit left and then read write 1/0 bit be placed in the LSB
{   int taddr = (int) addr    ;         // need to support extra addr width caused by shift left
    taddr = (taddr << 1)+0    ;         // +0 for write
    return  (i2c2_send_byte_ack((char) taddr))  ;
} 

// function 08
//basic I2C byte send and get NAck
int i2c2_send_byte_ack(char data)
{  int x=0;
   // wait for transmit register to empty
   // (in case there was a character being sent when we caLLE3D THIS))
   //while (I2C2STATbits.TBF)                                           NRM May 2 16. ignore TBF, do all from TRSTAT
   while (I2C2STATbits.TRSTAT)                                          // NRM wait for ACK time of current byte
   { if (x > I2C2_Bit_TimeOut)
     {  I2C2_Snd_Byte_cnt_TO4++ ;
        return I2C2_TIME_OUT4   ;
     }
     x++  ;
   }
   x=0 ;
   // TR STAT goes low, can load register and send more
   // register emptied by hardware; can now re-load
   IFS3bits.MI2C2IF = 0; // Clear Interrupt
   I2C2TRN = data; // load the outgoing data byte
   // reloaded, wait for transmission to complete
   // while (!I2C2STATbits.TRSTAT)                                      // Remove quadrature conditions between TBF and TRSTAT
   // { if (x > I2C2_Byte_TimeOut)
   //   {  I2C2_Snd_Byte_cnt_TO3++ ;
   //      return I2C2_TIME_OUT3   ;
   //   }
   //   x++  ;
   // }
   x=0 ;  
   // transmisasion complete. ready to check for ACK
   while (I2C2STATbits.TRSTAT) 
      { x++                 ;
        if(x > I2C2_Byte_TimeOut)
         {  I2C2_Snd_Byte_cnt_TO2++ ;
            return I2C2_TIME_OUT2   ;
         }
         
      }
   // .TRSTAT clears on receiving (N)ACK bit)
   // Check for NO_ACK from slave, abort if not found
   if (I2C2STATbits.ACKSTAT == 1)
   { // If NAK-> end of transmission. Stop
      // i2c2_stop()
      I2C2_Snd_Byte_cnt_NAK++ ;
      return I2C2_NAK         ;
   }
   I2C2_Snd_Byte_cnt_ACK++ ;
   return  I2C2_SUCCESS    ;   
 }

// function 09
//function reads data, returns the read data, with ack
int i2c2_read_byte_ack(int nack)	//does not reset bus!!!
{  int x = 0;
   while (I2C2CONbits.RCEN)          // Wait for RCEN to clear
    { if (x > I2C2_Byte_TimeOut)
     {  I2C2_Get_Byte_cnt_TO4++ ;
        return I2C2_TIME_OUT4   ;
     }
    }
   //set I2C module to receive      // RCEN set high again
   I2C2CONbits.RCEN = 1;
   //if no response, break
   x=0  ;
   // while (!I2C2STATbits.RBF)
   while (I2C2CONbits.RCEN)        // wait for this character to go
    { if (x > I2C2_Byte_TimeOut)
     {  I2C2_Get_Byte_cnt_TO3++ ;
        return I2C2_TIME_OUT3   ;
     }
     x++  ;
   }
   //get data from I2CRCV register
   I2C2_Data = I2C2RCV;
   //set ACK to high
   I2C2CONbits.ACKDT = nack;    //set to send ACK with 0  (1 is NAK)
   I2C2CONbits.ACKEN = 1;    // here we send ACK/NAK
   //wait before exiting
   x=0 ;
   while (I2C2CONbits.ACKEN)
    { if (x > I2C2_Bit_TimeOut)
     {  I2C2_Get_Byte_cnt_TO2++ ;
        return I2C2_TIME_OUT2   ;
     }
     x++  ;
   }
   // NOTE: Master sends ACK if slave to continue sending another byte OR
   // Master sends NAK to tewll slave stop sending.
   // There is no NAK response from a Receive byte string.
   //return data at pointer
   I2C2_Get_Byte_cnt_ACK++ ;
   return I2C2_SUCCESS     ;
}


    
// ****************************************************************************
//
//      byte level functions- end
//
//      String level functions-  begin
//
// ****************************************************************************

// function 11
int     i2c2_send_string(char addr, char I2C2_slen, char *I2C2_sbufptr)
{   int     s=0         ;
    int     status      ;
    int     data        ;
    
   while (s< I2C2_slen)
    {   data =  *I2C2_sbufptr         ;
        status = i2c2_send_byte_ack(data) ;   
        if (status > 0) 
        { status=  (s*16)+status    ;
           return (status)    ;
        }
        I2C2_sbufptr++  ;
        s++ ;
    }   
   return status ; 
}           ;

// function 12
int i2c2_read_string(char addr, char I2C2_rlen, char *I2C2_rbufptr)
{   int     r=0    ;
    int     status    ;
    
    while (r< I2C2_rlen)
    {   if (r < I2C2_rlen-1)
        {  status= i2c2_read_byte_ack(0)    ;
           if (status > 0) 
           { status= (r*16) + status     ;
             return (status)    ;
           }
           *I2C2_rbufptr= I2C2_Data ;
        }
        else
        { status = i2c2_read_byte_ack(1)    ;
          if (status > 0) 
          { status= (r*16) + status     ;
            return (status)    ;
          }
          *I2C2_rbufptr= I2C2_Data ;
        }   ;
        I2C2_rbufptr++  ;
        r++     ;
    }
    // i2c2_stop()         ;
    // i2c2_reset_bus ()   ;
    return status          ;
}

// ****************************************************************************
//
//      string level functions-  end
//
//      diagnostic level functions- begin
//
// ****************************************************************************

int i2c2_Diag_Report(void)
{   // Build the reporting Diagnostic STring
    I2C2_Diagnostics[0] =  I2C2_Msg_Ack_cnt       ; 
    I2C2_Diagnostics[1] =  I2C2_Msg_Nak_cnt       ;
    I2C2_Diagnostics[2] =  I2C2_Msg_Reset_cnt     ;
    I2C2_Diagnostics[3] =  I2C2_Msg_Req_cnt       ;
    
    I2C2_Diagnostics[4] =  I2C2_Snd_Byte_cnt_TO2  ;
    I2C2_Diagnostics[5] =  I2C2_Snd_Byte_cnt_TO3  ;
    I2C2_Diagnostics[6] =  I2C2_Snd_Byte_cnt_TO4  ;
    I2C2_Diagnostics[7] =  I2C2_Snd_Byte_cnt_ACK  ;
    I2C2_Diagnostics[8] =  I2C2_Snd_Byte_cnt_NAK  ;

    I2C2_Diagnostics[9] =  I2C2_Get_Byte_cnt_TO2  ;
    I2C2_Diagnostics[10]=  I2C2_Get_Byte_cnt_TO3  ;
    I2C2_Diagnostics[11]=  I2C2_Get_Byte_cnt_TO4  ;
    I2C2_Diagnostics[12]=  I2C2_Get_Byte_cnt_ACK  ;
    I2C2_Diagnostics[13]=  I2C2_Get_Byte_cnt_NAK  ;
    I2C2_Diagnostics[14]=  I2c2_STop_Bit_Width_X1 ;   // not used
    I2C2_Diagnostics[15]=  I2c2_STop_Bit_WidthCnt ;   // not used
    return (0)      ;
}

int i2c2_Diag_Clear(void)
{   // Clear all counters, and letting diags begin again
    I2C2_Msg_Ack_cnt        =0 ; 
    I2C2_Msg_Nak_cnt        =0 ;
    I2C2_Msg_Reset_cnt      =0 ;
    I2C2_Msg_Req_cnt        =0 ;
    
    I2C2_Snd_Byte_cnt_TO2   =0 ;
    I2C2_Snd_Byte_cnt_TO3   =0 ;
    I2C2_Snd_Byte_cnt_TO4   =0 ;
    I2C2_Snd_Byte_cnt_ACK   =0 ;
    I2C2_Snd_Byte_cnt_NAK   =0 ;

    I2C2_Get_Byte_cnt_TO2   =0 ;
    I2C2_Get_Byte_cnt_TO3   =0 ;
    I2C2_Get_Byte_cnt_TO4   =0 ;
    I2C2_Get_Byte_cnt_ACK   =0 ;
    I2C2_Get_Byte_cnt_NAK   =0 ;
 
    I2c2_STop_Bit_Width_X1  =0 ;    // used to ddetermine typical bit width at baud 
    I2c2_STop_Bit_Width_X2  =0 ;    // used to determine typical bit width at baud 
    I2c2_STop_Bit_WidthCnt  =0 ;    // used to determine typical bit width at baud 
///////////
   return (0)      ;
}

// ****************************************************************************
//
//      diagnostic level functions-  end
//
//      message level functions- begin
//
// ****************************************************************************

// function 14
//char i2c2_ping(char addr)
//{  // Hey address- ARE YOU THERE?
// {   int status = 0;
//    i2c2_start();
//    status = i2c2_send_addr_wt(addr);
//    // $ $ $ Get Ack NAk ?
//    i2c2_stop()             ;
//    i2c2_reset_bus()    ;
//    return (status)    ;
// }

// function 15
int i2c2_TalkToDevice(char addr, char I2C2_slen, char *I2C2_sbuf, char I2C2_rlen, char *I2C2_rbuf)
{   int lcount   =5   ;
    int stat     =0   ;
    int I2C2_stat=0   ;
    
    I2C2_Msg_Req_cnt++  ;
    if (addr == I2C_GET_DIAG )
    {  i2c2_Diag_Report()      ;
       return (0);                  // we done with this request
    }
if (addr == I2C_CLR_DIAG )
    {   i2c2_Diag_Clear()      ;
        return (0);                  // we done with this request
    }
    // could include an I2C2 Inbit call here if address = some variable value
    // if (I2C2_slen+I2C2_rlen == 0)
    //     return (I2C2_stat);                  // we done with this request
    // }
    while (lcount > 0)
    {  if (I2C2_stat >  0)       
        {  I2C2_Msg_Nak_cnt++ ;
        // clean up from previous loop attemp
        I2C2_Msg_Reset_cnt++  ;   
        delay(1)            ;
        i2c2_reset_bus ()   ;
        I2C2_stat  =  0     ;
        }
       if (I2C2_stat ==  0)
        {   stat= i2c2_start()  ;
            if(stat>0)
            { stat=stat <<4     ;
             I2C2_stat=stat+1   ; // encode error and where           }
            }
        }
        if ((I2C2_stat ==  0) && (I2C2_slen > 0))
        {   stat= i2c2_send_addr_wt(addr)   ;
            if(stat>0)
            { stat=stat <<4 ;
              I2C2_stat=stat+2   ; // encode error and where           }
            }
        }
      if ((I2C2_stat ==  0)  && (I2C2_slen > 0))
        {   stat= i2c2_send_string(addr, I2C2_slen, I2C2_sbuf)    ;
            if(stat>0)
            { stat=stat <<4 ;
             I2C2_stat=stat+3   ; // encode error and where           }
            }
        }   
       if ((I2C2_stat ==  0)  && (I2C2_rlen > 0)  && (I2C2_slen > 0))
        {   // check rlen to see that stuff sent and RESTART needed (if NO, START is all needed)
            // check slen to see that stuff to be sent and RESTART needed
            stat=    i2c2_restart()            ;
            if(stat>0)
            { stat=stat <<4 ;
              I2C2_stat=stat+4   ; // encode error and where           }
            }
        }
       if ((I2C2_stat ==  0)  && (I2C2_rlen > 0))
        {   stat=     i2c2_send_addr_rd(addr)   ;  
            if(stat>0)
            { stat=stat <<4 ;
              I2C2_stat=stat+5   ; // encode error and where           }
            }
        }
       if ((I2C2_stat ==  0)  && (I2C2_rlen > 0))
        {   stat=        i2c2_read_string(addr, I2C2_rlen, I2C2_rbuf)   ; 
            if(stat>0)
            { stat=stat <<4 ;
             I2C2_stat=stat+6   ; // encode error and where           }
            }
       }
      if ((I2C2_stat ==  0)  && (I2C2_rlen == 0)  && (I2C2_slen == 0))
       {   stat =       i2c2_send_addr_wt(addr);   // PING
           if(stat > 0) 
             { stat = stat << 4       ;
               I2C2_stat = stat+15    ;
             }
       }
       if (I2C2_stat ==  0)
        {   stat=     i2c2_stop()         ;  
            if(stat>0)
            { stat=stat <<4 ;
              I2C2_stat=stat+7   ; // encode error and where           }
            }
        }
       if (I2C2_stat == 0)
       {   I2C2_Msg_Ack_cnt++  ;
           break   ;
       }
       --lcount     ;
    }
    if (I2C2_stat != 0)
       {   I2C2_Msg_Nak_cnt++ ;
          i2c2_reset_bus ()   ;
          I2C2_stat  =  0     ;
       }
   i2c2_Diag_Report() ;
   return (I2C2_stat) ; 
}

/* 
 * File:   NVMDrivers.c
 * Author: Norman McFarland
 *
 * Created on January 11, 2017, 1:34 PM
 * 
 * Changes to file:
 * 
 * 
 */
// NVM is in External RTC device
// External RTC is accessed via I2C at I2C 7 bit device address 0x86
// NVM memory starts at device internal offset address 0x08 through 0x3F(8-64)
// xRTC data is at device internal offset address 0x00 through 0x07
// and is NOT accessed via NVM functions (See dateTimeRTC.c)


#include "system.h"
#include "nesi.h"
#include "../NormI2C/RealTimeClock.h"
#include "../NormI2C/I2CDrivers.h"

#define RlenMax 56      // Max bytes in xRTC NVM, can send direct to application
#define WlenMax 56      // internal buffer limit. extend buffer- extend limit

// NVM memory starts at device internal offset address 0x08 through 0x3F(8-64)
int readNVM(char *Dptr, Byte BgnAddr, Byte Rlen)
{   // Read values from xRTC NVM and place in application at supplied pointer
    char NVM_stg[2] = {0,0} ;
    int stat=0  ;   // capture I2C return status

    if(Rlen+BgnAddr > RlenMax+8) 
    { // Oops- to m,any bytes to send
        return (-1) ;
    }
    
    NVM_stg[0]  = BgnAddr  ;   // RTC Memory begin address
    // Call i2C to read data and put at pointer
    stat = i2c2_TalkToDevice(RTC_ADDR,1,NVM_stg,Rlen,Dptr)  ; //JRM
    return (stat)   ;
}

int writeNVM(char *Sptr, Byte BgnAddr, Byte Wlen)
{   // write up to 56 bytes to xRTC NVM
    // Note can not send direct from application. must include BgnAddr
    
    char NVM_stg[57] ; // BAddress +56
    initNVMBuffer(NVM_stg, 0);
    
    int stat=0  ;   // capture I2C return status
    int i   = 0 ;   // local variable

    if(Wlen+BgnAddr > WlenMax+8)    //prevent NVM from being overwritten
    { // Oops- to m,any bytes to send
        return (-1) ;
    }
    
    NVM_stg[0]  = BgnAddr  ;   // RTC Memory begin address
    for (i=1; i<=Wlen; i++)
    {
        NVM_stg[i] = Sptr[i-1]    ;//JRM        
//original        NVM_stg[i] = Sptr[i]    ; // RTC Memory begin address
    }
//original    stat= i2c2_TalkToDevice(RTC_ADDR, 1,NVM_stg,Wlen+1,Sptr)    ;
    stat= i2c2_TalkToDevice(RTC_ADDR, Wlen+1,NVM_stg,0,NullPtr); //JRM debug
    nop();
    nop();
    return(stat)    ;
}
 
#define BufferSize  56            
char Buffer1[BufferSize]    ; 
char Buffer2[BufferSize]    ; 
char Buffer3[BufferSize]    ; 

// Initializes 56 byte buffer to value passed "val" 
void initNVMBuffer(char *Ptr, char val)
{   int i  = 0 ;
    for (i=0; i<BufferSize; i++)
    {   Ptr[i]=val    ;
    }
    return  ;
}
//initializes 56 byte buffer to value passed "val" 
void setNVMBuffer(char *Ptr, char val)
{   
    int i  = 0 ;
    for (i=0; i<BufferSize; i++)
    {   Ptr[i]=val+i    ;
    }
    return  ;
}

// Compares two 56 byte buffers
// returns 0 if they are equivalent
// or value indicating which buffer location fails compare first(index+1)
int compareNVMBuffer(char *Ptr1, char *Ptr2)
{   
    int i  = 0 ;
    for (i=0; i<BufferSize; i++)
    {   if (Ptr1[i] != Ptr2[i]  )  
        {   return(i+1)       ;
        }
    }
    return(0)   ;
}

// int testNVM(void)
// Tests read/write to 56 byte NVM located in external RTC device
// NVM is located at internal offset 8 - 64, 0x08-0x3F
// External RTC is accessed via I2C at I2C 7 bit device address 0x86
// testNVM() will preserve current NVM memory, 
// then write a pattern of 0-56 into the NVM memory
// then read 56 byte content back and verify results are what was written

int testNVM(void)
{   int stat    = 0  ;  
    initNVMBuffer(Buffer1, 0)      ; // initialize buffers
    initNVMBuffer(Buffer2, 0)      ;
    initNVMBuffer(Buffer3, 0)      ;    
    
    readNVM(Buffer3, 8, 56)     ;   // Retain entire 56 byte NVRAM buffer
    
    setNVMBuffer(Buffer2,0)        ;   // load buffer 2 with count values, 0,1,2,3,...

    writeNVM (Buffer2,  8,  56)  ;   // write 56 byte pattern to NVRAM
 
    readNVM(Buffer1, 8, 56)     ;   // read all NVM bytes into Buffer

    stat = compareNVMBuffer(Buffer1, Buffer2)  ; //Compare what was written to what was read
    
    writeNVM (Buffer3,  8,  56)  ;   // Restore original contents of 56 byte NVRAM 
    
    return(stat)   ;   // returns 0 if match or first non matching character position        
}




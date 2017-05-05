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
#include "system.h"
#include "nesi.h"
#include "RealTimeClock.h"
#include "I2CDrivers.h"

#define RlenMax 56      // Max bytes in xRTC NVM, can send direct to application
#define WlenMax 16      // internal buffer limit. extend buffer- extend limit

int readNVM(char *Dptr, Byte BgnAddr, Byte Rlen)
{   // Read values from xRTC NVM and place in application at supplied pointer
    char NVM_stg[2] = {0,0} ;
    int stat=0  ;   // capture I2C return status

    if(Rlen > RlenMax)
    { // Oops- to m,any bytes to send
        return (-1) ;
    }
    
    NVM_stg[0]  = BgnAddr  ;   // RTC Memory begin address
    // Call i2C to read data and put at pointer
    stat = i2c2_TalkToDevice(RTC_ADDR,2,NVM_stg,Rlen,Dptr)  ;
    return (stat)   ;
}

int writeNVM(char *Sptr, Byte BgnAddr, Byte Wlen)
{   // write up to 16 bytes to xRTC NVM (more if extend local buffer)
    // Note can not send direct from application. must include BgnAddr
    
    char NVM_stg[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ; // BAddress +16 bytes
    int stat=0  ;   // capture I2C return status
    int i   = 0 ;   // local variable
            
    if(Wlen > WlenMax)
    { // Oops- to m,any bytes to send
        return (-1) ;
    }
    
    NVM_stg[0]  = BgnAddr  ;   // RTC Memory begin address
    for (i=1; i<=Wlen; i++)
    {   NVM_stg[i] = Sptr[i]    ; // RTC Memory begin address
    }
    stat= i2c2_TalkToDevice(RTC_ADDR, 1,NVM_stg,Wlen+1,Sptr)    ;
    return(stat)    ;
}
 
#define BufferSize  56            
char Buffer1[BufferSize]    ; 
char Buffer2[BufferSize]    ; 

void initBuffer(char *Ptr, char val)
{   int i  = 0 ;
    for (i=0; i<BufferSize; i++)
    {   Ptr[i]=val    ;
    }
    return  ;
}

void setBuffer(char *Ptr, char val)
{   
    int i  = 0 ;
    for (i=0; i<BufferSize; i++)
    {   Ptr[i]=val+i    ;
    }
    return  ;
}

int compareBuffer(char *Ptr1, char *Ptr2)
{   
    int i  = 0 ;
    for (i=0; i<BufferSize; i++)
    {   if (Ptr1[i] != Ptr2[i]  )  
        {   return(i+1)       ;
        }
    }
    return(0)   ;
}

// int readNVM(Byte* Dptr, Byte BgnAddr, Byte Rlen)
// int writeNVM (Byte* Sptr, Byte BgnAddr, Byte Wlen)

int TestNVM(void)
{   int stat    = 0  ;  
    initBuffer(Buffer1, 0)      ; // initialize bothe buffers
    initBuffer(Buffer1, 0)      ;
    
    setBuffer(Buffer2,0)        ;   // load buffer 2 with count values, 0,1,2,3,...
            
    readNVM(Buffer1, 8, 56)     ; // read all NVM bytes into Buffer
     
    writeNVM (Buffer2,  8,  8)  ;   // write first 8 bytes
    writeNVM (Buffer2, 16, 16)  ;   // write next 16 bytes
    writeNVM (Buffer2, 32, 16)  ;   // write next 16 bytes
    writeNVM (Buffer2, 48, 16)  ;   // write next 16 bytes
 
    readNVM(Buffer1, 8, 56)     ;   // read all NVM bytes into Buffer

    stat = compareBuffer(Buffer1, Buffer2)  ;
    
    return(stat)   ;   // returns 0 if match or first non matching character position        
}




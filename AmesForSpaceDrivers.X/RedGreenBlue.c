#include "system.h"
#include "nesi.h"
#include "PIC24_I2C2_R7.h"
#include "RedGreenBlue.h"

int SNS_CLR_LIGHT   = 0 ;
int SNS_RED_LIGHT   = 0 ;
int SNS_BLU_LIGHT   = 0 ;
int SNS_GRN_LIGHT   = 0 ;

const char RGB_MaxSend = 2        ;     // Max number of bytes you can send in one message
const char RGB_MaxRead = 128      ;     // Max number of bytes you can continuous read (Register size)

const char RGB_MinRegAddr  = 0x80 ;   // Lowest register address accessable from host
const char RGB_MaxRegAdsr  = 0xFF  ;   // Highest register address accessable from host

char    RGB_rbuf[32]               ;  // Read buffer
// CONFIG #1                  addr  byte1 byte2 byte3 byte4
const char  RGB_Cfg1L1      = 3                    ;
char        RGB_Cfg1S1[3]  = {0x80, 0x03, 0xFF}    ;    // Reset Chip  RGB_Cfg1P1
char*       RGB_Cfg1P1     = RGB_Cfg1S1            ;
const char  RGB_Cfg1L2     = 2                     ;
char        RGB_Cfg1S2[2]  = {0x83, 0xFF}    ;    
char*       RGB_Cfg1P2     = RGB_Cfg1S2            ;
const char  RGB_Cfg1L3     = 3                     ;
char        RGB_Cfg1S3[3]  = {0x8C, 0x00, 0x60}    ;    
char*       RGB_Cfg1P3     = RGB_Cfg1S3            ;
const char  RGB_Cfg1L4     = 3                     ;
char        RGB_Cfg1S4[3]  = {0x8F, 0xC1, 0x01}    ;    
char*       RGB_Cfg1P4     = RGB_Cfg1S4            ;
const char  RGB_Cfg1L5     = 5                     ;
char        RGB_Cfg1S5[5]  = {0x84, 0X00, 0X00, 0xFF, 0xFF}    ;    // Set Lower Upper Alarm limits
char*       RGB_Cfg1P5     = RGB_Cfg1S4            ;

// CONFIG #2    // dATASHEET tABLE 9; pG 18
const char  RGB_Cfg2L1     = 3                     ;
char        RGB_Cfg2S1[3]  = {0x80, 0x00, 0x00}    ;    // Reset Chip
char*       RGB_Cfg2P1     = RGB_Cfg2S1            ;
const char  RGB_Cfg2L2     = 2                     ;
char        RGB_Cfg2S2[2]  = {0x83, 0x00}    ;    
char*       RGB_Cfg2P2     = RGB_Cfg2S2            ;
const char  RGB_Cfg2L3     = 3                     ;
char        RGB_Cfg2S3[3]  = {0x8C, 0x00, 0x00}    ;    
char*       RGB_Cfg2P3     = RGB_Cfg2S3            ;
const char  RGB_Cfg2L4     = 3                     ;
char        RGB_Cfg2S4[3]  = {0x8F, 0x00, 0x00}    ;    
char*       RGB_Cfg2P4     = RGB_Cfg2S4            ;
const char  RGB_Cfg2L5     = 5                     ;
char        RGB_Cfg2S5[5]  = {0x84, 0X00, 0X00, 0xFF, 0xFF}    ;    // Set Lower Upper Alarm limits
char*       RGB_Cfg2P5     = RGB_Cfg2S4            ;


// CONFIG #3    // dATASHEET tABLE 9; pG 18
const char  RGB_Cfg3L1     = 3                     ;
char        RGB_Cfg3S1[3]  = {0x80, 0x00, 0x00}    ;    // Reset Chip
char*       RGB_Cfg3P1     = RGB_Cfg3S1            ;
const char  RGB_Cfg3L2     = 2                     ;
char        RGB_Cfg3S2[2]  = {0x83, 0x00}    ;    
char*       RGB_Cfg3P2     = RGB_Cfg3S2            ;
const char  RGB_Cfg3L3     = 3                     ;
char        RGB_Cfg3S3[3]  = {0x8C, 0x00, 0x00}    ;    
char*       RGB_Cfg3P3     = RGB_Cfg3S3            ;
const char  RGB_Cfg3L4     = 3                     ;
char        RGB_Cfg3S4[3]  = {0x8F, 0x00, 0x00}    ;    
char*       RGB_Cfg3P4     = RGB_Cfg3S4            ;
const char  RGB_Cfg3L5     = 5                     ;
char        RGB_Cfg3S5[5]  = {0x84, 0X00, 0X00, 0xFF, 0xFF}    ;    // Set Lower Upper Alarm limits
char*       RGB_Cfg3P5     = RGB_Cfg3S4            ;

// - - - - - - - - - - - - - - - - - - - - - - 

// Clear 4 Irq's by writing to 4 registers- any value           

const char  RGB_ClrIrqLR     = 5                   ;
char        RGB_ClrIrqSR[5]  = {0xE4, 0X00, 0X00, 0X00, 0x00}    ;    // Irq Register Set
char*       RGB_ClrIrqPR     = RGB_ClrIrqSR        ;

// Send Register Address
const char  RGB_StatusLS   = 1               ;
char        RGB_StatusSS[1]  = {0x93}        ;
char*       RGB_StatusP   = RGB_StatusSS     ;

// Get Status Register contens
const char  RGB_StatusL1   = 1               ;
char        RGB_Status     = 0               ;  // (Actual memory byte))
char*       RGB_StatusP1   = &RGB_Status      ;

const char  RGB_RdValL1     = 1               ;
char        RGB_RdValS1[2]  = {0x93, 0x00}    ;    // Second Register Set    
char*       RGB_RdValP1     = RGB_RdValS1      ;

// Not used:
const char  RGB_Cmp1L3     = 1               ;
char        RGB_Cmp1S3[2]  = {0xF2, 0x00}    ;    // Third Register Set    
char*       RGB_Cmp1P3     = RGB_Cmp1S3      ;

const char  RGB_Cmp1L4     = 1               ;
char        RGB_Cmp1S4[2]  = {0xF7, 0x00}    ;    // Data Register Set    
char*       RGB_Cmp1P4     = RGB_Cmp1S4      ;


int RGB_Configure1 (void)
{ int RGB_stat  = 0 ;
      RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg1L1, RGB_Cfg1S1, 0, NullPtr)    ;    // Reg *0
  if (RGB_stat != 0)
  {   RGB_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg1L2,RGB_Cfg1S2,0,NullPtr)    ;       // Reg 83
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg1L3,RGB_Cfg1S3,0,NullPtr)    ;        // Reg 8C
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg1L4,RGB_Cfg1S4,0,NullPtr)    ;       // Reg 8F
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  64  ;       //  Function #4 Failed
  }
  else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg1L5,RGB_Cfg1S5,0,NullPtr)    ;   // set ALS limits
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  128  ;       //  Function #4 Failed
  }
  return (RGB_stat)     ;
 }

int RGB_Configure2 (void)
{ int RGB_stat  = 0 ;
      RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg2L1, RGB_Cfg2P1, 0, NullPtr)    ;
  if (RGB_stat != 0)
  {   RGB_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg2L2,RGB_Cfg2P2,0,NullPtr)    ;
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg2L3,RGB_Cfg2P3,0,NullPtr)    ;
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg2L4,RGB_Cfg2P4,0,NullPtr)    ;
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  64  ;       //  Function #4 Failed
  }
  else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg2L5,RGB_Cfg2P5,0,NullPtr)    ;
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  1218  ;       //  Function #4 Failed
  }
  return (RGB_stat)     ;
 }

int RGB_Configure3 (void)
{ int RGB_stat  = 0 ;
      RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg3L1, RGB_Cfg3P1, 0, NullPtr)    ;
  if (RGB_stat != 0)
  {   RGB_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg3L2,RGB_Cfg3P2,0,NullPtr)    ;
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg3L3,RGB_Cfg3P3,0,NullPtr)    ;
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg3L4,RGB_Cfg3P4,0,NullPtr)    ;
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  64  ;       //  Function #4 Failed
  }
  else
  {   RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_Cfg3L5,RGB_Cfg3P5,0,NullPtr)    ;
  } 
  if (RGB_stat != 0)
  {   RGB_stat +=  128  ;       //  Function #4 Failed
  }
  return (RGB_stat)     ;
}

int RGB_ClearIrqs (void)
{ int RGB_stat  = 0 ;
  RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_ClrIrqLR, RGB_ClrIrqSR, 0, NullPtr)    ;
  return (RGB_stat)     ;
 }

int RGB_ReadStatus (void)
{ int RGB_stat  = 0 ;
  RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_StatusLS, RGB_StatusSS, RGB_StatusL1, RGB_rbuf+16)    ;
  return (RGB_stat)     ;
 }

int RGB_ReadValues (void)
{   int RGB_stat  = 0 ;
    int x = 0 ;
  RGB_stat= i2c2_TalkToDevice(RGB_Addr, RGB_RdValL1, RGB_RdValS1, 9, RGB_rbuf)    ;
  if ( (RGB_rbuf[0]  && 0x01) == 0)
  { x++     ;
  }
  else      
  { SNS_CLR_LIGHT   = (RGB_rbuf[2]*256) + RGB_rbuf[1] ;   // Low High x95 0x94    RGB_rbuf[2], RGB_rbuf[1]
    SNS_RED_LIGHT   = (RGB_rbuf[4]*256) + RGB_rbuf[3] ;   // Low High x97 0x96    RGB_rbuf[], RGB_rbuf[]
    SNS_GRN_LIGHT   = (RGB_rbuf[6]*256) + RGB_rbuf[5] ;   // Low High x99 0x98    RGB_rbuf[], RGB_rbuf[]
    SNS_BLU_LIGHT   = (RGB_rbuf[8]*256) + RGB_rbuf[7] ;   // Low High x9B 0x9A    RGB_rbuf[], RGB_rbuf[] 
  }
 return (RGB_stat)     ;
 }



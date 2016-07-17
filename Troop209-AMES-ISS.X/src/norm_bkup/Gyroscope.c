#include "system.h"
#include "nesi.h"
#include "PIC24_I2C2_R7.h"
#include "Gyroscope.h"




const char GYR_MaxSend = 64        ;     // Max number of bytes you can send in one message
const char GYR_MaxRead = 64      ;     // Max number of bytes you can continuous read (Register size)

const char GYR_MinRegAddr  = 0x00 ;   // Lowest register address accessable from host
const char GYR_MaxRegAdsr  = 0x3F  ;   // Highest register address accessable from host

char    GYR_rbuf[16]               ;  // Read buffer
// CONFIG #1                  addr  byte1 byte2 byte3 byte4
const char  GYR_Cfg1L1      = 7                    ;
char        GYR_Cfg1S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip  GYR_Cfg1P1
char*       GYR_Cfg1P1     = GYR_Cfg1S1            ;
const char  GYR_Cfg1L2     = 2                     ;
char        GYR_Cfg1S2[2]  = {0x2E, 0x1F}    ;    
char*       GYR_Cfg1P2     = GYR_Cfg1S2            ;
const char  GYR_Cfg1L3     = 2                     ;
char        GYR_Cfg1S3[2]  = {0x30, 0x00}    ;    
char*       GYR_Cfg1P3     = GYR_Cfg1S3            ;
const char  GYR_Cfg1L4     = 7                     ;
char        GYR_Cfg1S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       GYR_Cfg1P4     = GYR_Cfg1S4            ;

// CONFIG #2    // dATASHEET tABLE 9; pG 18
const char  GYR_Cfg2L1     = 7                     ;
char        GYR_Cfg2S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip
char*       GYR_Cfg2P1     = GYR_Cfg2S1            ;
const char  GYR_Cfg2L2     = 2                     ;
char        GYR_Cfg2S2[2]  = {0x2E, 0x1F}    ;    
char*       GYR_Cfg2P2     = GYR_Cfg2S2            ;
const char  GYR_Cfg2L3     = 2                     ;
char        GYR_Cfg2S3[2]  = {0x30, 0x00}    ;    
char*       GYR_Cfg2P3     = GYR_Cfg2S3            ;
const char  GYR_Cfg2L4     = 7                     ;
char        GYR_Cfg2S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       GYR_Cfg2P4     = GYR_Cfg2S4            ;


// CONFIG #3    // dATASHEET tABLE 9; pG 18
const char  GYR_Cfg3L1     = 7                     ;
char        GYR_Cfg3S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip
char*       GYR_Cfg3P1     = GYR_Cfg3S1            ;
const char  GYR_Cfg3L2     = 2                     ;
char        GYR_Cfg3S2[2]  = {0x2E, 0x1F}    ;    
char*       GYR_Cfg3P2     = GYR_Cfg3S2            ;
const char  GYR_Cfg3L3     = 2                     ;
char        GYR_Cfg3S3[2]  = {0x30, 0x00}    ;    
char*       GYR_Cfg3P3     = GYR_Cfg3S3            ;
const char  GYR_Cfg3L4     = 7                     ;
char        GYR_Cfg3S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       GYR_Cfg3P4     = GYR_Cfg3S4            ;

// - - - - - - - - - - - - - - - - - - - - - - 
// Get GYro Status byte
const char  GYR_StatusLS   = 1               ;
char        GYR_StatusSS[1]  = {0x27}        ;
char*       GYR_StatusP   = GYR_StatusSS     ;
// Get Status Register contents
const char  GYR_StatusL1   = 1               ;
char        GYR_Status     = 0               ;  // (Actual memory byte))
char*       GYR_StatusP1   = &GYR_Status      ;

// Get Gyro Data           

const char  GYR_RdValLS1     = 1                   ;
char        GYR_RdValSR[1]  = {0xA8}    ;    // Irq Register Set
char*       GYR_RdValPR1     = GYR_RdValSR        ;

const char  GYR_RdValLR1     = 6                  ;

// DIAG read back config registers
const char  GYR_RdCfgLS     = 1                   ;
char        GYR_RdCfgSR[1]  = {0xA8}    ;    // Irq Register Set
char*       GYR_RdCfgPR     = GYR_RdCfgSR        ;

const char  GYR_RdCfgLR     = 6                  ;

// DIAG read back Interrupt Request registers
const char  GYR_RdIrqsLS     = 1                   ;
char        GYR_RdIrqsSR[1]  = {0xA8}    ;    // Irq Register Set
char*       GYR_RdIrqsPR     = GYR_RdIrqsSR        ;

const char  GYR_RdIrqsLR     = 8                  ;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//               wORK Time
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int GYR_Configure1 (void)
{ int GYR_stat  = 0 ;

      GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg1L1, GYR_Cfg1S1, 0, NullPtr)    ;    // Reg *0
  if (GYR_stat != 0)
  {   GYR_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg1L2, GYR_Cfg1S2, 0, NullPtr)    ;       // Reg 83
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg1L3, GYR_Cfg1S3, 0, NullPtr)    ;        // Reg 8C
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg1L4, GYR_Cfg1S4, 0, NullPtr)    ;       // Reg 8F
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  64  ;       //  Function #4 Failed
  }
  return (GYR_stat)     ;
 }

int GYR_Configure2 (void)
{ int GYR_stat  = 0 ;

      GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg2L1, GYR_Cfg2P1, 0, NullPtr)    ;
  if (GYR_stat != 0)
  {   GYR_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg2L2,GYR_Cfg2P2,0,NullPtr)    ;
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg2L3,GYR_Cfg2P3,0,NullPtr)    ;
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg2L4,GYR_Cfg2P4,0,NullPtr)    ;
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  64  ;       //  Function #4 Failed
  }
  return (GYR_stat)     ;
 }

int GYR_Configure3 (void)
{ int GYR_stat  = 0 ;

      GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg3L1, GYR_Cfg3P1, 0, NullPtr)    ;
  if (GYR_stat != 0)
  {   GYR_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg3L2,GYR_Cfg3P2,0,NullPtr)    ;
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg3L3,GYR_Cfg3P3,0,NullPtr)    ;
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_Cfg3L4,GYR_Cfg3P4,0,NullPtr)    ;
  } 
  if (GYR_stat != 0)
  {   GYR_stat +=  64  ;       //  Function #4 Failed
  }
  return (GYR_stat)     ;
}

int GYR_ReadStatus (void)
{ int GYR_stat  = 0 ;

  GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_StatusLS, GYR_StatusP, GYR_StatusL1, GYR_StatusP1)    ;
  return (GYR_stat)     ;
 }

int GYR_ReadValues (void)
{ int GYR_stat  = 0 ;

  GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_RdValLS1, GYR_RdValPR1, GYR_RdValLR1, GYR_rbuf)    ;
  return (GYR_stat)     ;
 }

int GYR_Diag_RdIrqs (void)
{ int GYR_stat  = 0 ;

  GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_RdIrqsLS, GYR_RdIrqsPR, GYR_RdIrqsLR, GYR_rbuf)    ;
  return (GYR_stat)     ;
 }

int GYR_Diag_RdCfgs (void)
{ int GYR_stat  = 0 ;

  GYR_stat= i2c2_TalkToDevice(GYR_Addr, GYR_RdCfgLS, GYR_RdCfgPR, GYR_RdCfgLR, GYR_rbuf)    ;
  return (GYR_stat)     ;
 }




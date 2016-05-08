#include "system.h"
#include "nesi.h"
#include "PIC24_I2C2_R6.h"
#include "Magnetometer.h"

// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $$ $ $ $ $ $
//              `SHELL PROGRAM FOr Magnetometer
//                No COn fig data placed yet
// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $$ $ $ $ $ $


const char MAG_MaxSend = 64        ;     // Max number of bytes you can send in one message
const char MAG_MaxRead = 64      ;     // Max number of bytes you can continuous read (Register size)

const char MAG_MinRegAddr  = 0x00 ;   // Lowest register address accessable from host
const char MAG_MaxRegAdsr  = 0x3F  ;   // Highest register address accessable from host

char    MAG_rbuf[16]               ;  // Read buffer
// CONFIG #1                  addr  byte1 byte2 byte3 byte4
const char  MAG_Cfg1L1      = 7                    ;
char        MAG_Cfg1S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip  MAG_Cfg1P1
char*       MAG_Cfg1P1     = MAG_Cfg1S1            ;
const char  MAG_Cfg1L2     = 2                     ;
char        MAG_Cfg1S2[2]  = {0x2E, 0x1F}    ;    
char*       MAG_Cfg1P2     = MAG_Cfg1S2            ;
const char  MAG_Cfg1L3     = 2                     ;
char        MAG_Cfg1S3[2]  = {0x30, 0x00}    ;    
char*       MAG_Cfg1P3     = MAG_Cfg1S3            ;
const char  MAG_Cfg1L4     = 7                     ;
char        MAG_Cfg1S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       MAG_Cfg1P4     = MAG_Cfg1S4            ;

// CONFIG #2    // dATASHEET tABLE 9; pG 18
const char  MAG_Cfg2L1     = 7                     ;
char        MAG_Cfg2S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip
char*       MAG_Cfg2P1     = MAG_Cfg2S1            ;
const char  MAG_Cfg2L2     = 2                     ;
char        MAG_Cfg2S2[2]  = {0x2E, 0x1F}    ;    
char*       MAG_Cfg2P2     = MAG_Cfg2S2            ;
const char  MAG_Cfg2L3     = 2                     ;
char        MAG_Cfg2S3[2]  = {0x30, 0x00}    ;    
char*       MAG_Cfg2P3     = MAG_Cfg2S3            ;
const char  MAG_Cfg2L4     = 7                     ;
char        MAG_Cfg2S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       MAG_Cfg2P4     = MAG_Cfg2S4            ;


// CONFIG #3    // dATASHEET tABLE 9; pG 18
const char  MAG_Cfg3L1     = 7                     ;
char        MAG_Cfg3S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip
char*       MAG_Cfg3P1     = MAG_Cfg3S1            ;
const char  MAG_Cfg3L2     = 2                     ;
char        MAG_Cfg3S2[2]  = {0x2E, 0x1F}    ;    
char*       MAG_Cfg3P2     = MAG_Cfg3S2            ;
const char  MAG_Cfg3L3     = 2                     ;
char        MAG_Cfg3S3[2]  = {0x30, 0x00}    ;    
char*       MAG_Cfg3P3     = MAG_Cfg3S3            ;
const char  MAG_Cfg3L4     = 7                     ;
char        MAG_Cfg3S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       MAG_Cfg3P4     = MAG_Cfg3S4            ;

// - - - - - - - - - - - - - - - - - - - - - - 
// Get GYro Status byte
const char  MAG_StatusLS   = 1               ;
char        MAG_StatusSS[1]  = {0x27}        ;
char*       MAG_StatusP   = MAG_StatusSS     ;
// Get Status Register contents
const char  MAG_StatusL1   = 1               ;
char        MAG_Status     = 0               ;  // (Actual memory byte))
char*       MAG_StatusP1   = &MAG_Status      ;

// Get Gyro Data           

const char  MAG_RdValLS1     = 1                   ;
char        MAG_RdValSR[1]  = {0xA8}    ;    // Irq Register Set
char*       MAG_RdValPR1     = MAG_RdValSR        ;

const char  MAG_RdValLR1     = 6                  ;

// DIAG read back config registers
const char  MAG_RdCfgLS     = 1                   ;
char        MAG_RdCfgSR[1]  = {0xA8}    ;    // Irq Register Set
char*       MAG_RdCfgPR     = MAG_RdCfgSR        ;

const char  MAG_RdCfgLR     = 6                  ;

// DIAG read back Interrupt Request registers
const char  MAG_RdIrqsLS     = 1                   ;
char        MAG_RdIrqsSR[1]  = {0xA8}    ;    // Irq Register Set
char*       MAG_RdIrqsPR     = MAG_RdIrqsSR        ;

const char  MAG_RdIrqsLR     = 8                  ;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//               wORK Time
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int MAG_Configure1 (void)
{ int MAG_stat  = 0 ;

      MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg1L1, MAG_Cfg1S1, 0, NullPtr)    ;    // Reg *0
  if (MAG_stat != 0)
  {   MAG_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg1L2, MAG_Cfg1S2, 0, NullPtr)    ;       // Reg 83
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg1L3, MAG_Cfg1S3, 0, NullPtr)    ;        // Reg 8C
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg1L4, MAG_Cfg1S4, 0, NullPtr)    ;       // Reg 8F
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  64  ;       //  Function #4 Failed
  }
  return (MAG_stat)     ;
 }

int MAG_Configure2 (void)
{ int MAG_stat  = 0 ;

      MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg2L1, MAG_Cfg2P1, 0, NullPtr)    ;
  if (MAG_stat != 0)
  {   MAG_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg2L2,MAG_Cfg2P2,0,NullPtr)    ;
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg2L3,MAG_Cfg2P3,0,NullPtr)    ;
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg2L4,MAG_Cfg2P4,0,NullPtr)    ;
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  64  ;       //  Function #4 Failed
  }
  return (MAG_stat)     ;
 }

int MAG_Configure3 (void)
{ int MAG_stat  = 0 ;

      MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg3L1, MAG_Cfg3P1, 0, NullPtr)    ;
  if (MAG_stat != 0)
  {   MAG_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg3L2,MAG_Cfg3P2,0,NullPtr)    ;
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg3L3,MAG_Cfg3P3,0,NullPtr)    ;
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_Cfg3L4,MAG_Cfg3P4,0,NullPtr)    ;
  } 
  if (MAG_stat != 0)
  {   MAG_stat +=  64  ;       //  Function #4 Failed
  }
  return (MAG_stat)     ;
}

int MAG_ReadStatus (void)
{ int MAG_stat  = 0 ;

  MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_StatusLS, MAG_StatusP, MAG_StatusL1, MAG_StatusP1)    ;
  return (MAG_stat)     ;
 }

int MAG_ReadValues (void)
{ int MAG_stat  = 0 ;

  MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_RdValLS1, MAG_RdValPR1, MAG_RdValLR1, MAG_rbuf)    ;
  return (MAG_stat)     ;
 }

int MAG_Diag_RdIrqs (void)
{ int MAG_stat  = 0 ;

  MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_RdIrqsLS, MAG_RdIrqsPR, MAG_RdIrqsLR, MAG_rbuf)    ;
  return (MAG_stat)     ;
 }

int MAG_Diag_RdCfgs (void)
{ int MAG_stat  = 0 ;

  MAG_stat= i2c2_TalkToDevice(MAG_Addr, MAG_RdCfgLS, MAG_RdCfgPR, MAG_RdCfgLR, MAG_rbuf)    ;
  return (MAG_stat)     ;
 }




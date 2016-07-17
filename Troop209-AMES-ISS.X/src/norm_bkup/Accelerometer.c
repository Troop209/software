#include "system.h"
#include "nesi.h"
#include "PIC24_I2C2_R7.h"
#include "Accelerometer.h"
// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $$ $ $ $ $ $
//              `SHELL PROGRAM FOr Accelerometer
//                No COn fig data placed yet
// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $$ $ $ $ $ $


const char ACL_MaxSend = 64        ;     // Max number of bytes you can send in one message
const char ACL_MaxRead = 64      ;     // Max number of bytes you can continuous read (Register size)

const char ACL_MinRegAddr  = 0x00 ;   // Lowest register address accessable from host
const char ACL_MaxRegAdsr  = 0x3F  ;   // Highest register address accessable from host

char    ACL_rbuf[16]               ;  // Read buffer
// CONFIG #1                  addr  byte1 byte2 byte3 byte4
const char  ACL_Cfg1L1      = 7                    ;
char        ACL_Cfg1S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip  ACL_Cfg1P1
char*       ACL_Cfg1P1     = ACL_Cfg1S1            ;
const char  ACL_Cfg1L2     = 2                     ;
char        ACL_Cfg1S2[2]  = {0x2E, 0x1F}    ;    
char*       ACL_Cfg1P2     = ACL_Cfg1S2            ;
const char  ACL_Cfg1L3     = 2                     ;
char        ACL_Cfg1S3[2]  = {0x30, 0x00}    ;    
char*       ACL_Cfg1P3     = ACL_Cfg1S3            ;
const char  ACL_Cfg1L4     = 7                     ;
char        ACL_Cfg1S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       ACL_Cfg1P4     = ACL_Cfg1S4            ;

// CONFIG #2    // dATASHEET tABLE 9; pG 18
const char  ACL_Cfg2L1     = 7                     ;
char        ACL_Cfg2S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip
char*       ACL_Cfg2P1     = ACL_Cfg2S1            ;
const char  ACL_Cfg2L2     = 2                     ;
char        ACL_Cfg2S2[2]  = {0x2E, 0x1F}    ;    
char*       ACL_Cfg2P2     = ACL_Cfg2S2            ;
const char  ACL_Cfg2L3     = 2                     ;
char        ACL_Cfg2S3[2]  = {0x30, 0x00}    ;    
char*       ACL_Cfg2P3     = ACL_Cfg2S3            ;
const char  ACL_Cfg2L4     = 7                     ;
char        ACL_Cfg2S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       ACL_Cfg2P4     = ACL_Cfg2S4            ;


// CONFIG #3    // dATASHEET tABLE 9; pG 18
const char  ACL_Cfg3L1     = 7                     ;
char        ACL_Cfg3S1[7]  = {0xA0, 0xFF, 0x20, 0x10, 0x10, 0x00, 0x00}    ;    // Reset Chip
char*       ACL_Cfg3P1     = ACL_Cfg3S1            ;
const char  ACL_Cfg3L2     = 2                     ;
char        ACL_Cfg3S2[2]  = {0x2E, 0x1F}    ;    
char*       ACL_Cfg3P2     = ACL_Cfg3S2            ;
const char  ACL_Cfg3L3     = 2                     ;
char        ACL_Cfg3S3[2]  = {0x30, 0x00}    ;    
char*       ACL_Cfg3P3     = ACL_Cfg3S3            ;
const char  ACL_Cfg3L4     = 7                     ;
char        ACL_Cfg3S4[7]  = {0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    ;    
char*       ACL_Cfg3P4     = ACL_Cfg3S4            ;

// - - - - - - - - - - - - - - - - - - - - - - 
// Get GYro Status byte
const char  ACL_StatusLS   = 1               ;
char        ACL_StatusSS[1]  = {0x27}        ;
char*       ACL_StatusP   = ACL_StatusSS     ;
// Get Status Register contents
const char  ACL_StatusL1   = 1               ;
char        ACL_Status     = 0               ;  // (Actual memory byte))
char*       ACL_StatusP1   = &ACL_Status      ;

// Get Gyro Data           

const char  ACL_RdValLS1     = 1                   ;
char        ACL_RdValSR[1]  = {0xA8}    ;    // Irq Register Set
char*       ACL_RdValPR1     = ACL_RdValSR        ;

const char  ACL_RdValLR1     = 6                  ;

// DIAG read back config registers
const char  ACL_RdCfgLS     = 1                   ;
char        ACL_RdCfgSR[1]  = {0xA8}    ;    // Irq Register Set
char*       ACL_RdCfgPR     = ACL_RdCfgSR        ;

const char  ACL_RdCfgLR     = 6                  ;

// DIAG read back Interrupt Request registers
const char  ACL_RdIrqsLS     = 1                   ;
char        ACL_RdIrqsSR[1]  = {0xA8}    ;    // Irq Register Set
char*       ACL_RdIrqsPR     = ACL_RdIrqsSR        ;

const char  ACL_RdIrqsLR     = 8                  ;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//               wORK Time
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int ACL_Configure1 (void)
{ int ACL_stat  = 0 ;

      ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg1L1, ACL_Cfg1S1, 0, NullPtr)    ;    // Reg *0
  if (ACL_stat != 0)
  {   ACL_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg1L2, ACL_Cfg1S2, 0, NullPtr)    ;       // Reg 83
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg1L3, ACL_Cfg1S3, 0, NullPtr)    ;        // Reg 8C
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg1L4, ACL_Cfg1S4, 0, NullPtr)    ;       // Reg 8F
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  64  ;       //  Function #4 Failed
  }
  return (ACL_stat)     ;
 }

int ACL_Configure2 (void)
{ int ACL_stat  = 0 ;

      ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg2L1, ACL_Cfg2P1, 0, NullPtr)    ;
  if (ACL_stat != 0)
  {   ACL_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg2L2,ACL_Cfg2P2,0,NullPtr)    ;
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg2L3,ACL_Cfg2P3,0,NullPtr)    ;
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg2L4,ACL_Cfg2P4,0,NullPtr)    ;
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  64  ;       //  Function #4 Failed
  }
  return (ACL_stat)     ;
 }

int ACL_Configure3 (void)
{ int ACL_stat  = 0 ;

      ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg3L1, ACL_Cfg3P1, 0, NullPtr)    ;
  if (ACL_stat != 0)
  {   ACL_stat +=  16  ;       //  Function #1 Failed
  }
 else
  {   ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg3L2,ACL_Cfg3P2,0,NullPtr)    ;
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  32  ;       //  Function #2 Failed
  }
  else
  {  ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg3L3,ACL_Cfg3P3,0,NullPtr)    ;
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  48  ;       //  Function #3 Failed
  }
  else
  {   ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_Cfg3L4,ACL_Cfg3P4,0,NullPtr)    ;
  } 
  if (ACL_stat != 0)
  {   ACL_stat +=  64  ;       //  Function #4 Failed
  }
  return (ACL_stat)     ;
}

int ACL_ReadStatus (void)
{ int ACL_stat  = 0 ;

  ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_StatusLS, ACL_StatusP, ACL_StatusL1, ACL_StatusP1)    ;
  return (ACL_stat)     ;
 }

int ACL_ReadValues (void)
{ int ACL_stat  = 0 ;

  ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_RdValLS1, ACL_RdValPR1, ACL_RdValLR1, ACL_rbuf)    ;
  return (ACL_stat)     ;
 }

int ACL_Diag_RdIrqs (void)
{ int ACL_stat  = 0 ;

  ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_RdIrqsLS, ACL_RdIrqsPR, ACL_RdIrqsLR, ACL_rbuf)    ;
  return (ACL_stat)     ;
 }

int ACL_Diag_RdCfgs (void)
{ int ACL_stat  = 0 ;

  ACL_stat= i2c2_TalkToDevice(ACL_Addr, ACL_RdCfgLS, ACL_RdCfgPR, ACL_RdCfgLR, ACL_rbuf)    ;
  return (ACL_stat)     ;
 }




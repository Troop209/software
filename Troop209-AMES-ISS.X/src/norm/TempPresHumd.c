#include "system.h"
#include "nesi.h"
#include "PIC24_i2c2_R5.h"
#include "TempPresHumd.h"

signed int SNS_Temerature   ;
signed int SNS_Pressure     ;
signed int SNS_Humidity     ;

signed int Raw_Temerature   ;
signed int Raw_Pressure     ;
signed int Raw_Humidity     ;

const char THP_MaxSend = 2        ;     // Max number of bytes you can send in one message
const char THP_MaxRead = 128      ;     // Max number of bytes you can continuous read (Register size)

const char THP_MinRegAddr  = 0x80 ;   // Lowest register address accessable from host
const char THP_MaxRegAdsr  = 0xFF ;   // Highest register address accessable fro H M L 0 0 H L 2 3 4 5  

// Raw Values and Diagnostics buffer
//  fNL fORMAT: P P P P T T T T H H H H d d d d 
//  fNL fORMAT: s H M L s H M L s s H L 2 3 4 5  
//  bGN fORMAT: P P P T T T H H d d d d . . .
//  cNT oFFSET: 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5

// Configuration data (2 sets)
//   T   T   T   P   P   P   P   P   P   P   P   P   H   H   H   H   H   H 
//   1   2   3   1   2   3   4   5   6   7   8   9 x 1   2   3   4   5   6 
//   T   T   T   P   P   P   P   P   P   P   P   P   H   H H   H H H . . .      
//   T   T   T   P   P   P   P   P   P   P   P   P   H   H   H   H   H   H 
//   1   2   3   1   2   3   4   5   6   7   8   9 x 1   2   3   4   5   6 
//   T   T   T   P   P   P   P   P   P   P   P   P   H   H H   H H H       
//   1   2   3   1   2   3   4   5   6   7   8   9 x 1   2 3   4 5 6       
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7


struct ConfigParams {
    unsigned short  dig_T1   ;   // From THP Register 0x88 to 0xA1
    signed short    dig_T2   ;
    signed short    dig_T3   ;
    unsigned short  dig_P1   ;
    signed short    dig_P2   ;
    signed short    dig_P3   ;
    signed short    dig_P4   ;
    signed short    dig_P5   ;
    signed short    dig_P6   ;
    signed short    dig_P7   ;
    signed short    dig_P8   ;
    signed short    dig_P9   ;
    unsigned char   dig_nu   ;
    unsigned char   dig_H1   ;   // 0xA1 H1 LSB in MSB position
    signed short    dig_H2   ;   //0XE1
    unsigned char   dig_H3   ;
    signed short    dig_H4   ;  // These fields calculated after fill
    signed short    dig_H5   ;
    signed   char   dig_H6   ; 
 }  ConfigParams;  
    
struct ConfigParams Config12  ;

    // short* Cfg12Ptr = &Config12 ; 
    
    
const char THP_RdBufLen = 64 ;
char THP_rbuf[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0,0,0}    ;
char THP_Data[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0} ;
//                      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
const char THP_Cfg1_Ofs  =  0                   ;
const char THP_Cfg2_Ofs  = 26                   ;
const char THP_Cfg3_Ofs  = 36                   ;
const char THP_Cfg4_Ofs  = 40                   ;

// CONFIG #1
const char THP_Cfg1L1      = 2               ;
char        THP_Cfg1S1[2]  = {0x80, 0x6B}    ;    // Reset Chip  THP_Cfg1P1
char*       THP_Cfg1P1     = THP_Cfg1S1      ;
const char  THP_Cfg1L2     = 2               ;
char        THP_Cfg1S2[2]  = {0xF2, 0x01}    ;    
char*       THP_Cfg1P2     = THP_Cfg1S2      ;
const char  THP_Cfg1L3     = 2               ;
char        THP_Cfg1S3[2]  = {0xF5, 0x84}    ;    
char*       THP_Cfg1P3     = THP_Cfg1S3      ;
const char  THP_Cfg1L4     = 2               ;
char        THP_Cfg1S4[2]  = {0xF4, 0x27}    ;    
char*       THP_Cfg1P4     = THP_Cfg1S4      ;

// CONFIG #2    // DATASHEET TABLE 9; pG 18
const char  THP_Cfg2L1     = 8               ;
char        THP_Cfg2S1[8]  = {0x80, 0x6B, 0xF2, 0x01, 0xF5, 0x10, 0xF4, 0x27}    ;    // Reset Chip disabled should be B6 to reset not 6B. 6B ignorede by THP
char*       THP_Cfg2P1     = THP_Cfg2S1      ;
const char  THP_Cfg2L2     = 2               ;
char        THP_Cfg2S2[2]  = {0xF2, 0x01}    ;    
char*       THP_Cfg2P2     = THP_Cfg2S2      ;
const char  THP_Cfg2L3     = 2               ;
char        THP_Cfg2S3[2]  = {0xF5, 0x10}    ;    
char*       THP_Cfg2P3     = THP_Cfg2S3      ;
const char  THP_Cfg2L4     = 2               ;
char        THP_Cfg2S4[2]  = {0xF4, 0x54}    ;    
char*       THP_Cfg2P4     = THP_Cfg2S4      ;


// CONFIG #3    // Extra- just in case needed
const char  THP_Cfg3L1     = 2               ;
char        THP_Cfg3S1[2]  = {0x80, 0x6B}    ;    // Reset Chip
char*       THP_Cfg3P1     = THP_Cfg3S1      ;
const char  THP_Cfg3L2     = 2               ;
char        THP_Cfg3S2[2]  = {0xF2, 0x00}    ;    
char*       THP_Cfg3P2     = THP_Cfg3S2      ;
const char  THP_Cfg3L3     = 2               ;
char        THP_Cfg3S3[2]  = {0xF5, 0x10}    ;    
char*       THP_Cfg3P3     = THP_Cfg3S3      ;
const char  THP_Cfg3L4     = 2               ;
char        THP_Cfg3S4[2]  = {0xF4, 0x03}    ;    
char*       THP_Cfg3P4     = THP_Cfg3S4      ;
// - - - - - - - - - - - - - - - - - - - - - - 

// COMPENSATION Fetch Addresses            
const char  THP_Cmp1L1     = 1               ;
char        THP_Cmp1S1[1]  = {0x88}    ;    // First Configuration Parameter Register Set
char*       THP_Cmp1P1     = THP_Cmp1S1      ;
const char  THP_Cmp1L2     = 1               ;
char        THP_Cmp1S2[1]  = {0xE1}    ;    // Second Configuration Parameter Register Set    
char*       THP_Cmp1P2     = THP_Cmp1S2      ;
const char  THP_Cmp1L3     = 1               ;
char        THP_Cmp1S3[1]  = {0xF2}    ;    // Third Register Set- Configs   
char*       THP_Cmp1P3     = THP_Cmp1S3      ;
const char  THP_Cmp1L4     = 1               ;
char        THP_Cmp1S4[1]  = {0xF7}    ;    // Data Register Set    
char*       THP_Cmp1P4     = THP_Cmp1S4      ;

int THP_Configure1 (void)
{ int THP_stat  = 0 ;
  int THP_diag  = 0 ;
      // Send COnfiguration 1 to device. No response for configuration data
      // THP_SetBuffer(THP_rbuf, THP_RdBufLen, 0xFF)                            ;
      // THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg1L1, THP_Cfg1S1, 0, NullPtr) ;
      //. THP_diag= i2c2_TalkToDevice(THP_Addr,          1, THP_Cfg1S1, 1, THP_rbuf) ;
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   1  ;       //  Function #1 Failed
  }
 else
  {   THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg1L2,THP_Cfg1S2,0,NullPtr)    ;
      THP_diag= i2c2_TalkToDevice(THP_Addr,          1,THP_Cfg1S2, 1, THP_rbuf)    ;
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   2  ;       //  Function #2 Failed
  }
  else
  {   // THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg1L3,THP_Cfg1S3,0,NullPtr)    ;
      // THP_diag= i2c2_TalkToDevice(THP_Addr,          1,THP_Cfg1S3, 1, THP_rbuf)    ;
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   3  ;       //  Function #3 Failed
  }
  else
  {   // THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg1L4,THP_Cfg1S4,0,NullPtr)    ;
      // THP_diag= i2c2_TalkToDevice(THP_Addr,          1,THP_Cfg1S4, 1, THP_rbuf)    ;
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   4  ;       //  Function #4 Failed
  }
  return (THP_stat)     ;
 }

int THP_Configure2 (void)
{ int THP_stat  = 0 ;
      THP_SetBuffer(THP_rbuf, THP_RdBufLen, 0x55)                            ;
      THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg2L1, THP_Cfg2S1, 0, NullPtr) ;
      THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L3, THP_Cmp1S3, 8, THP_Data+8)    ; // Read back config data
  return (THP_stat)     ;
 }

int THP_Configure3 (void)
{ int THP_stat  = 0 ;
      THP_SetBuffer(THP_rbuf, 64, 0xFF)                            ;
      THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg3L1, THP_Cfg3S1, 0, NullPtr) ;
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   1  ;       //  Function #1 Failed
  }
 else
  {   THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg3L2,THP_Cfg3S2,0,NullPtr)    ;
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   2  ;       //  Function #2 Failed
  }
  else
  {  THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg3L3,THP_Cfg3S3,0,NullPtr)    ;
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   3  ;       //  Function #3 Failed
  }
  else
  {   THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg3L4,THP_Cfg3S4,0,NullPtr)    ;
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   4  ;       //  Function #4 Failed
  }
  return (THP_stat)     ;
}


int THP_Read_Values (void)
{ int THP_stat  = 0 ;
     THP_SetBuffer(THP_rbuf, THP_RdBufLen, 0x55)                            ;
     // Send Begin Register Address then Read remote register values
      THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L1, THP_Cmp1S1, 26, THP_rbuf)    ;
      if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   1  ;       //  Read Block 1 Config Values Function #1 Failed
  }
 else
  {  // Get second sert of COnfig Parameters
      THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L2, THP_Cmp1S2, 7, THP_rbuf+26)    ;
      THP_LoadConfigs ()    ;   // move buffer info to fields
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   2  ;       //  Read block 2 config values Function #2 Failed
  }
  else
  {  // Get the status values
     THP_SetBuffer(THP_Data, THP_RdBufLen, 0x55)                            ;
     THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L3, THP_Cmp1S3, 4, THP_Data)    ;    // Read Status

     // $ $ $ Harmon Function Calls Here     ;
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   3  ;       //  Function #3 Failed
  }
  else
  {   // Lets get data valkues
      THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L4, THP_Cmp1S4, 8, THP_Data)    ;  // Read Data
      THP_LoadRawValues () ;
  } 
  if (THP_stat != 0)
  {   THP_stat = (THP_stat << 4) +   4  ;       //  Function #4 Failed
  }
   
  return (THP_stat)     ;
  }

    
void THP_LoadConfigs (void) 
{   ConfigParams.dig_T1   = THP_rbuf[1]*256  + THP_rbuf[0];      // From THP Register 0x88 to 0xA1
    ConfigParams.dig_T2   = THP_rbuf[3]*256  + THP_rbuf[2];
    ConfigParams.dig_T3   = THP_rbuf[5]*256  + THP_rbuf[4];
    ConfigParams.dig_P1   = THP_rbuf[7]*256  + THP_rbuf[6];
    ConfigParams.dig_P2   = THP_rbuf[9]*256  + THP_rbuf[8];
    ConfigParams.dig_P3   = THP_rbuf[11]*256 + THP_rbuf[10];
    ConfigParams.dig_P4   = THP_rbuf[13]*256 + THP_rbuf[12];
    ConfigParams.dig_P5   = THP_rbuf[15]*256 + THP_rbuf[14];
    ConfigParams.dig_P6   = THP_rbuf[17]*256 + THP_rbuf[16];
    ConfigParams.dig_P7   = THP_rbuf[19]*256 + THP_rbuf[18];
    ConfigParams.dig_P8   = THP_rbuf[21]*256 + THP_rbuf[20];
    ConfigParams.dig_P9   = THP_rbuf[23]*256 + THP_rbuf[22];
    ConfigParams.dig_H1   = THP_rbuf[25]*256 + THP_rbuf[24];
    ConfigParams.dig_H2   = THP_rbuf[27]*256 + THP_rbuf[26];   // 0xA1 H1 LSB in MSB position
    ConfigParams.dig_H3   =               0*256 + THP_rbuf[28];   //0XE1
    ConfigParams.dig_H4   = THP_rbuf[30]*256 + THP_rbuf[29];
    ConfigParams.dig_H5   =               0*256 + THP_rbuf[31];
    ConfigParams.dig_H6   = THP_rbuf[35]*256 + THP_rbuf[34];  // 0xE7 End of data fill
}         


// Returns temperature in DegC, resolution is 0.01 DegC. Output value of ?5123? equals 51.23 DegC.
// t_fine carries fine temperature as global value 
signed long t_fine;
signed long THP_compensate_T_int32(signed long adc_T)
{  signed long var1, var2, T;
// 0000000111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990000000000111111111122222222223
// 4567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
   var1  = (((adc_T>>3) - ((signed long)   ConfigParams.dig_T1 << 1) * ((signed long)  ConfigParams.dig_T2 ) ) >> 11) ;
   var2  = ((((adc_T>>4) - ((signed long)   ConfigParams.dig_T1    )) * ((adc_T>>4) - ((signed long)ConfigParams.dig_T1)) >> 12) *
           ((signed long)ConfigParams.dig_T3)) >> 14;
   t_fine = var1 + var2;
   T  = (t_fine * 5 + 128) >> 8;
return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa 
unsigned long THP_compensate_P_int64(signed long adc_P)
{  signed long long var1, var2, p;
    var1 = (((signed long long) t_fine) - 128000)  ;
    var2 = var1 * var1 * (signed long long)ConfigParams.dig_P6;
    var2 = var2 + ((var1*(signed long long)ConfigParams.dig_P5)<<17);
    var2 = var2 + (((signed long long)ConfigParams.dig_P4)<<35);
    var1 = ((var1 * var1 * (signed long long)ConfigParams.dig_P3)>>8) + ((var1 * (signed long long)ConfigParams.dig_P2)<<12);
    var1 = (((((signed long long)1)<<47)+var1))*((signed long long)ConfigParams.dig_P1)>>33;
    if (var1 == 0)  {   return 0;    // avoid exception caused by division by zero  
    }  
    p = 1048576-adc_P;
    p = (((p<<31)-var2)*3125)/var1;
    var1 = (((signed long long)ConfigParams.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((signed long long)ConfigParams.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((signed long long)ConfigParams.dig_P7)<<4);
return (unsigned long)p  ;
}  
// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of "47445" represents 47445/1024 = 46.333 %RH
unsigned long THP_compensate_H_int32(signed long adc_H)
{  signed long v_x1_u32r;
    v_x1_u32r = (t_fine - ((signed long)76800));
    v_x1_u32r = (((((adc_H << 14) - (((signed long)ConfigParams.dig_H4) << 20) - (((signed long)ConfigParams.dig_H5) * v_x1_u32r)) + ((signed long)16384)) >> 15) * (((((((v_x1_u32r * ((signed long)ConfigParams.dig_H6)) >> 10) * (((v_x1_u32r * 
                ((signed long)ConfigParams.dig_H3)) >> 11) + ((signed long)32768))) >> 10) + ((signed long)2097152)) * ((signed long)ConfigParams.dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((signed long)ConfigParams.dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return (unsigned long)(v_x1_u32r>>12);
} 

// Raw Values and Diagnostics buffer
//  fNL fORMAT: P P P P T T T T H H H H d d d d 
//  fNL fORMAT: s H M L s H M L s s H L 2 3 4 5  
//  bGN fORMAT: P P P T T T H H d d d d . . .
//  cNT oFFSET: 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
void  THP_LoadRawValues (void)   
{     THP_LoadConfigs () ;
   // $ $ $ CORRECT SIGN   
   // Temperature    = 0b00000000MMMMMMMMLLLLLLLLXXXX0000 ;
      Raw_Temerature =  THP_Data[3]*65536 + THP_Data[4]*256 + THP_Data[5];
      SNS_Temerature = THP_compensate_T_int32((signed long) Raw_Temerature) ;

    // Pressure      = 0b00000000MMMMMMMMLLLLLLLLXXXX0000 ;
      Raw_Pressure   = THP_Data[0]*65536 + THP_Data[1]*256 + THP_Data[2];
      SNS_Pressure   = THP_compensate_P_int64((signed long) Raw_Pressure) ;
   // $ $ $ CORRECT SIGN
   // Humidirty      = 0b00000000MMMMMMMMLLLLLLLLXXXX0000 ;
      Raw_Humidity   =                     THP_Data[6]*256 + THP_Data[7];
      SNS_Humidity   = THP_compensate_H_int32((signed long) Raw_Humidity) ;
      return    ;
}

void THP_SetBuffer(char* Lcl_ReadBuf, int SetLen, char Fill)
{
    int i = 0 ;
    for (i=0; i< SetLen; i++)
    { Lcl_ReadBuf[0] =Fill ;
      Lcl_ReadBuf++     ;
    }
    return ;
}

// Following function SHOULD NOT BE NEEDED
void THP_SwapBytes (char* Lcl_ReadBuf, int Pairs)
{   int i    = 0    ;
    int temp = 0    ;
    for (i=0; i< Pairs; i++)
    { temp          = Lcl_ReadBuf[0]     ;
      Lcl_ReadBuf[0] = Lcl_ReadBuf[1]     ;   
      Lcl_ReadBuf[1] = temp              ;
      Lcl_ReadBuf++                     ;      
      Lcl_ReadBuf++                     ;      
    }
    
}

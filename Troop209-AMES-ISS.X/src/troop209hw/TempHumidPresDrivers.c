/* 
 * File:   TempHumidPresDrivers.c
 * Author: Norman McFarland
 *
 * Created before Nov 19, 2016, 3:10 PM
 * 
 * Revision Notes
 * Jan 17, 2017
 * 1. (Andrew Frank) The THP temperature calculations have been altered to produce correct 
values. I am going to check values for pressure and humidity soon.
 * 2. 
 */


#include "system.h"
#include "nesi.h"
#include "I2CDrivers.h"
#include "TempHumidPresDrivers.h"
#include "stdio.h"      // for printf statement(s) and file I/O
// #include "file.h"

int Combine2(char v1, char v2)    ;
long Combine3(char v1, char v2, char v3) ;

signed long Raw_Temperature   ;
signed long Raw_Pressure     ;
signed long Raw_Humidity     ;

float   FLT_Temperature   ;
float   FLT_Pressure     ;
float   FLT_Humidity     ;


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
char THP_rbuf[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}    ;
char THP_Data[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0} ;
//                      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
const char THP_Cfg1_Ofs  =  0                   ;
const char THP_Cfg2_Ofs  = 26                   ;
const char THP_Cfg3_Ofs  = 36                   ;
const char THP_Cfg4_Ofs  = 40                   ;

// - - - - - - - - - - - - - - - - - - - - - - 


// Constants for ReadValues           
const char  THP_Cmp1L1     = 1               ;
char        THP_Cmp1S1[1]  = {0x88}    ;    // First Configuration Parameter Register Set
const char  THP_Cmp1L2     = 1               ;
char        THP_Cmp1S2[1]  = {0xE1}    ;    // Second Configuration Parameter Register Set    
const char  THP_Cmp1L3     = 1               ;
char        THP_Cmp1S3[1]  = {0xF2}    ;    // Third Register Set- Configs   
const char  THP_Cmp1L4     = 1               ;
char        THP_Cmp1S4[1]  = {0xF7}    ;    // Data Register Set    

// Init Constants    // DATASHEET TABLE 9; pG 18
const char  THP_Cfg2L1     = 8               ;
char        THP_Cfg2S1[8]  = {0x80, 0x6B, 0xF2, 0x01, 0xF5, 0x10, 0xF4, 0x27}    ;    // Reset Chip disabled should be B6 to reset not 6B. 6B ignorede by THP
const char  THP_Cfg2L2     = 2               ;
char        THP_Cfg2S2[2]  = {0xF2, 0x01}    ;    
const char  THP_Cfg2L3     = 2               ;
char        THP_Cfg2S3[2]  = {0xF5, 0x10}    ;    
const char  THP_Cfg2L4     = 2               ;
char        THP_Cfg2S4[2]  = {0xF4, 0x54}    ;    


// Loads a default value into the buffer. Allows seeing what did or did not change
void THP_SetBuffer(char* Lcl_ReadBuf, int SetLen, char Fill)
{
    int i = 0 ;
    for (i=0; i< SetLen; i++)
    { Lcl_ReadBuf[0] =Fill ;
      Lcl_ReadBuf++     ;
    }
    return ;
}

// Function to combine two (signed) byte fields into 1 double byte int field
int Combine2(char v1, char v2)
{int rslt=0 ;
    rslt= v1;
    if(v2 < 0)
        {   rslt= v1+1;
        } else 
        {   rslt= v1;    
        }
    rslt= (rslt << 8);
    rslt= rslt + v2;
return (rslt);        
}

// Function to combine three (signed) byte fields into long int field
// note that lower 4 bits of v3 are not used and result is 20 bits, sign extended
long Combine3(char v1, char v2, char v3)
{long rslt=0 ;
    rslt= v1;
    if(v2 < 0)
        {   rslt= v1+1;
        } else 
        {   rslt= v1;    
        }
    rslt= (rslt << 8);
    rslt= rslt + v2;
    if(v3 < 0)
        {   rslt= rslt+1;
        } else 
        {  // no action   
        }
    rslt= (rslt << 8);
    rslt= rslt + v3;
return (rslt);        
}

// Load byte values into config parameter table
void THP_LoadConfigs (void) 
{   int h4LSB=((THP_rbuf[30] & 0x0F)<< 4) ;
    int h5LSB= (THP_rbuf[30] & 0xF0) ;
    ConfigParams.dig_T1   = Combine2(THP_rbuf[1], THP_rbuf[0]);      // From THP Register 0x88 to 0xA1
    ConfigParams.dig_T2   = Combine2(THP_rbuf[3], THP_rbuf[2]);
    ConfigParams.dig_T3   = Combine2(THP_rbuf[5], THP_rbuf[4]);
    ConfigParams.dig_P1   = Combine2(THP_rbuf[7], THP_rbuf[6]);
    ConfigParams.dig_P2   = Combine2(THP_rbuf[9], THP_rbuf[8]);
    ConfigParams.dig_P3   = Combine2(THP_rbuf[11], THP_rbuf[10]);
    ConfigParams.dig_P4   = Combine2(THP_rbuf[13], THP_rbuf[12]);
    ConfigParams.dig_P5   = Combine2(THP_rbuf[15], THP_rbuf[14]);
    ConfigParams.dig_P6   = Combine2(THP_rbuf[17], THP_rbuf[16]);
    ConfigParams.dig_P7   = Combine2(THP_rbuf[19], THP_rbuf[18]);
    ConfigParams.dig_P8   = Combine2(THP_rbuf[21], THP_rbuf[20]);
    ConfigParams.dig_P9   = Combine2(THP_rbuf[23], THP_rbuf[22]);
    ConfigParams.dig_H1   = Combine2(           0, THP_rbuf[25]);   
    ConfigParams.dig_H2   = Combine2(THP_rbuf[27], THP_rbuf[26]);   
    ConfigParams.dig_H3   = Combine2(           0, THP_rbuf[28]);   
    ConfigParams.dig_H4   = Combine2(THP_rbuf[29], h4LSB);   // 12 bit
    ConfigParams.dig_H5   = Combine2(THP_rbuf[31], h5LSB);   // 12 bit
    ConfigParams.dig_H6   = Combine2(THP_rbuf[33], THP_rbuf[32]);  
}         

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of ?5123? equals 51.23 DegC.
// t_fine carries fine temperature as global value 
signed long t_fine;
signed long varT1, varT2, T;
signed long THP_compensate_T_int32(signed long adc_T)
{  // signed long varT1, varT2, T;
// 0000000111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990000000000111111111122222222223
// 4567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
   varT1  = ((((adc_T>>3) - ((signed long)   ConfigParams.dig_T1 << 1))) * ((signed long)  ConfigParams.dig_T2 ) ) >> 11 ;
   varT2  = ((((adc_T>>4) - ((signed long)   ConfigParams.dig_T1    )) * ((adc_T>>4) - ((signed long)ConfigParams.dig_T1)) >> 12) *
           ((signed long)ConfigParams.dig_T3)) >> 14;
   t_fine = varT1 + varT2;
   T  = (t_fine * 5 + 128) >> 8;
return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa 
signed long long var1, var2, var3, var4, var5, var6, var7, var8, p;

unsigned long THP_compensate_P_int64(signed long adc_P)
{  //signed long long var1, var2, var3, var4, var5, var6, var7, var8, p;
    // var1, var2, var2, var2, var1, var1
    var1 = (((signed long long) t_fine) - 128000)  ;
    var2 = var1 * var1 * (signed long long)ConfigParams.dig_P6;
    var3 = var2 + ((var1*(signed long long)ConfigParams.dig_P5)<<17);
    var4 = var3 + (((signed long long)ConfigParams.dig_P4)<<35);
    var5 = ((var1 * var1 * (signed long long)ConfigParams.dig_P3)>>8) + ((var1 * (signed long long)ConfigParams.dig_P2)<<12);
    var6 = (((((signed long long)1)<<47)+var5))*((signed long long)ConfigParams.dig_P1)>>33;
    if (var6 == 0)  
    {   return 0;    // avoid exception caused by division by zero  
    }  
    p = 1048576-adc_P;
    p = (((p<<31)-var4)*3125)/var6;
    var7 = (((signed long long)ConfigParams.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var8 = (((signed long long)ConfigParams.dig_P8) * p) >> 19;
    p = ((p + var7 + var8) >> 8) + (((signed long long)ConfigParams.dig_P7)<<4);
return (unsigned long)p  ;
}  
// Returns pressure in Pa as double. Output value of "96386.2" equals 96386.2 Pascals or 963.862 hPa
double far1, far2, far3, far4, far5, far6, far7, far8, fp;
double THP_compenstate_P_double(adc_P)
{// double far1, far2, far3, far4, far5, far6, far7, far8, fp;
    // far1, far2, far2, far2, far1, far1
    far1= (((double)t_fine/(double)2.0)-(double)64000.0)  ;
    far2 = far1 * far1 * ((double)ConfigParams.dig_P6) / 32768.0 ;
    far3 = far2 + far1 * ((double)ConfigParams.dig_P5) * 2.0 ;
    far4 = (far3/4.0)+(((double)ConfigParams.dig_P4) * 65536.0) ;
    far5 = (((double)ConfigParams.dig_P3) * far1 * far1 / 524288.0 + 
           ((double)ConfigParams.dig_P2) * far1) / 524288 ; 
    far6 = (1.0 + far5 / 32768.0) * ((double)ConfigParams.dig_P1);
    if (far6 == 0.0)
    { return (0)    ;    // avoid divide-by-0 exception
    }
    fp = 1048576.0 - (double)adc_P   ;
    fp=(fp- (far2 / 4096.0)) *6250.0 / far6 ;
    far7 = ((double)ConfigParams.dig_P9) * fp * fp /2147483648.0 ;
    far8 = fp * ((double)ConfigParams.dig_P8) / 32768.0 ;
    fp = fp + (far7 + far8 + ((double)ConfigParams.dig_P7)) / 16 ;
    return (fp)  ;
}
         
// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of "47445" represents 47445/1024 = 46.333 %RH
signed long hum1, hum2, hum3, hum4 ;
unsigned long THP_compensate_H_int32(signed long adc_H)
 {  //signed long hum1, hum2, hum3, hum4 ;
    hum1 = (t_fine - ((signed long)76800));
    hum2 =     (((((adc_H << 14) - 
                   (((signed long)ConfigParams.dig_H4) << 20) - 
                   (((signed long)ConfigParams.dig_H5) * hum1)) + 
                   ((signed long)16384)) >> 15) * 
                (((((((hum1 * ((signed long)ConfigParams.dig_H6)) >> 10) * 
                   (((hum1 * ((signed long)ConfigParams.dig_H3)) >> 11) + 
                   ((signed long)32768))) >> 10) + 
                   ((signed long)2097152)) * 
                   ((signed long)ConfigParams.dig_H2) + 
                   8192) >> 14));
    hum3 = (hum2 - (((((hum2 >> 15) * 
                   (hum2 >> 15)) >> 7) * 
                   ((signed long)ConfigParams.dig_H1)) >> 4));
    hum4 = (hum3 < 0 ? 0 : hum3);
    hum4 = (hum3 > 419430400 ? 419430400 : hum3);
    return (unsigned long)(hum4>>12);
} 

// Returns humidity in %rH as a double. Output value of 46.332" represents 46.332 %rh
double far_H1, far_H2, far_H3, far_H4   ;
double compensate_H_double(long adc_H)
{       // double far_H1, far_H2, far_H3, far_H4   ;
    far_H1 = ((double)t_fine) -768000.0 ;
    far_H2 = (adc_H - 
            (((double)ConfigParams.dig_H4) * 64.0 + 
            ((double)ConfigParams.dig_H5) / 16384.0 * far_H1)) *
            (((double)ConfigParams.dig_H2) / 65536.0 * 
            (1.0 +((double)ConfigParams.dig_H6) / 67108864.0 *far_H1 *
            (1.0 + ((double)ConfigParams.dig_H3) / 67108864.0 * far_H1))) ;
    far_H3 = far_H2 * (1.0 - ((double)ConfigParams.dig_H1) * far_H2 / 524288.0) ;
    far_H4 = far_H3 ; 
    if (far_H3 > 100.0)
        far_H4 = 100.0   ;
    if (far_H3 < 0.0)
        far_H4 = 0.0   ;
    return (far_H4)  ;
}

// Raw Values and Diagnostics buffer
//  fNL fORMAT: P P P P T T T T H H H H d d d d 
//  fNL fORMAT: s H M L s H M L s s H L 2 3 4 5  
//  bGN fORMAT: P P P T T T H H d d d d . . .
//  cNT oFFSET: 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5
void  THP_LoadRawValues (void)   
{     THP_LoadConfigs () ;

    extern signed long SNS_Temperature   ;
    extern signed long SNS_Pressure     ;
    extern signed long SNS_Humidity     ;


   // $ $ $ CORRECT SIGN   
     // Pressure      = 0b00000000MMMMMMMMLLLLLLLLXXXX0000 ;
      Raw_Pressure   = (Combine3(THP_Data[0], THP_Data[1], THP_Data[2])) >> 4 ;
      SNS_Pressure   = THP_compensate_P_int64((signed long) Raw_Pressure) ;
      FLT_Pressure   = THP_compenstate_P_double(Raw_Pressure)   ;

      // Temperature    = 0b00000000MMMMMMMMLLLLLLLLXXXX0000 ;
      Raw_Temperature =  (Combine3(THP_Data[3], THP_Data[4], THP_Data[5])) >> 4 ;
      SNS_Temperature = THP_compensate_T_int32((signed long) Raw_Temperature) ;
      FLT_Temperature =  THP_compenstate_P_double(Raw_Temperature) ;

   // $ $ $ CORRECT SIGN
   // Humidirty      = 0b00000000MMMMMMMMLLLLLLLLXXXX0000 ;
      Raw_Humidity   =                     THP_Data[6]*256 + THP_Data[7];
      SNS_Humidity   = THP_compensate_H_int32((signed long) Raw_Humidity) ;
      FLT_Humidity   = compensate_H_double(Raw_Humidity)   ;  
      
      
      return    ;
}

char recBuf[512] ;
void diagRecord(void)
{   int i = 0;
    int len=0 ;
    for (i=0; i < 512; i++)
    {  recBuf[i]=0   ;    // set each char to end-of-string
    }

    len=sprintf(recBuf, "1,%ld,%ld,%ld,%ld,%ld,%ld,%ld,|T,%u,%d,%d,|P,%u,%d,%d,%d,%d,%d,%d,%d,%d,%u,|H,%u,%d,%u,%d,%d,%d|,|VF,%Ld,%f,%Ld,%f,%Ld,%f,%Ld,%f,%Ld,%f,|,%Ld,%f,%Ld,%f,%Ld,%f,%Ld,%f,|,%ld,%f,%ld,%f,%ld,%f,%ld,%f\n",
        Raw_Temperature, Raw_Pressure, Raw_Humidity, varT1, varT2, T, t_fine,
        ConfigParams.dig_T1, ConfigParams.dig_T2, ConfigParams.dig_T3, ConfigParams.dig_P1, 
        ConfigParams.dig_P2, ConfigParams.dig_P3, ConfigParams.dig_P4, ConfigParams.dig_P5, 
        ConfigParams.dig_P6, ConfigParams.dig_P7, ConfigParams.dig_P8, ConfigParams.dig_P9, 
        ConfigParams.dig_nu, ConfigParams.dig_H1, ConfigParams.dig_H2, ConfigParams.dig_H3, 
        ConfigParams.dig_H4, ConfigParams.dig_H5, ConfigParams.dig_H6,
        var1, far1, var2, far2, var3, far3, var4, far4, var5, far5, 
        var6, far6, var7, far7, var8, far8, p, fp,
        hum1, far_H1, hum2, far_H2,  hum3, far_H3,  hum4, far_H4 ) ;

 /*   
    file1.open("THPdata.csv");
    file1.write((Byte *)RecBuf, len);
    file1.close()   ;             
*/
    return  ;
}    

int initTHP (void)
{ int THP_stat  = 0 ;
      THP_SetBuffer(THP_rbuf, THP_RdBufLen, 0x55)                            ;
      THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cfg2L1, THP_Cfg2S1, 0, NullPtr) ;
      THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L3, THP_Cmp1S3, 8, THP_Data+8)    ; // Read back config data
  return (THP_stat)     ;
 }


int readTHP (void)
{   int THP_stat  = 0 ;

    THP_SetBuffer(THP_rbuf, THP_RdBufLen, 0x55)                            ;
     // Send Begin Register Address then Read remote register values
    THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L1, THP_Cmp1S1, 26, THP_rbuf)    ;
    if (THP_stat != 0)
    {   THP_stat = (THP_stat << 4) +   1  ;       //  Read Block 1 Config Values Function #1 Failed
    }
    else
    {  // Get second set of COnfig Parameters
          THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L2, THP_Cmp1S2, 7, THP_rbuf+26)    ;
          THP_LoadConfigs ()    ;   // move buffer info to fields
    } 
    if (THP_stat != 0)
    {   THP_stat = (THP_stat << 4) +   2  ;       //  Read block 2 config values Function #2 Failed
    }
    else
    {  // Get the status values
        THP_SetBuffer(THP_Data, THP_RdBufLen, 0x55)                            ;
        THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L3, THP_Cmp1S3, 4, THP_Data+8)    ;    // Read Status
        // $ $ $ Harmon Function Calls Here     ;
    }  
    if (THP_stat != 0)
    {   THP_stat = (THP_stat << 4) +   3  ;       //  Function #3 Failed
    }
    else
    {   // Lets get data values
        THP_stat= i2c2_TalkToDevice(THP_Addr, THP_Cmp1L4, THP_Cmp1S4, 8, THP_Data)    ;  // Read Data
        THP_LoadRawValues () ;
        // diagRecord () ;  // optional diag: write tons oif info to sd card record-
    } 
    if (THP_stat != 0)
    {   THP_stat = (THP_stat << 4) +   4  ;       //  Function #4 Failed
    }
   
  return (THP_stat)     ;
  }

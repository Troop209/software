/* 
 * File:   SensorDrivers.c
 * Author: Norman McFarland
 *
 * Created on November 20, 2016, 1:34 PM
 * 
 * Changes to file:
 * Rev 1 set Feb 099 2017
 * 1.1  add new Global var SNS_EncDirFlag // captures direction encoder reports on motor movements
 * 
 */
#include "system.h"
#include "nesi.h"

#include "RadiationSensorDriver.h"

#include "RedGreenBlueDrivers.h"
#include "TempHumidPresDrivers.h"
#include "dateTime_RTC.h"
#include "RadiationSensorDriver.h"
#include "Encoder.h"
#include "I2CDrivers.h"
#include "SensorDrivers.h"
#include "SensorErrorCodes.h"
#include "stdio.h"


// Error Codes = FncnXxxx + DrvrXxxxx + ErrXxxx + I2c2Xxxx (See I2CDrivers.h)

    #define BufLenOverflow  6 ;

    // TempHumPres Externals:
    signed long SNS_Temperature   ;
    signed long SNS_Pressure     ;
    signed long SNS_Humidity     ;

    //RedGreenBlue externals:
    int SNS_CLR_LIGHT   = 0 ;
    int SNS_RED_LIGHT   = 0 ;
    int SNS_BLU_LIGHT   = 0 ;
    int SNS_GRN_LIGHT   = 0 ;

    int SNS_AltLight    = 0 ;
    int SNS_AltTemp     = 0 ;
    
    // Encoder externals

    int SNS_virtEncoderBgn= 0   ;  
    int SNS_virtEncoderEnd= 0   ;
    int SNS_virtEncoderNow= 0   ;
    
    unsigned int SNS_EncPeriod    =  0 ;   // encoder PWM period
    unsigned int SNS_EncodPos     =  0 ;   // Current encoder position
    unsigned int SNS_EncodPosPrev =  0 ;   // Previous Encoder position
    unsigned int SNS_MovFlg       =  0 ;   // Is carousel moving? 1 Yes; 0 No
             int SNS_EncodOffset  =  0 ;   // when motor commanded to 180 deg, where encoder, calc 0 offset
             int SNS_EncDirFlag   =  0 ;   // 0 not set; +1 Encoder moves CW; -1 Encoder moves CCW

    unsigned int SNS_RadPeriod    =  0 ;   // Radiation Senasor PWM period

    // MotorController externals
    int SNS_VpotCur     =   -3000   ;   // Current Vpot value +- 1 count. Guarenetee first time update
    int SNS_MotorPos    =   -180    ;   // Current commanded motor position 
    int SNS_MotorDes    =   -180    ;   // Degrees to Destination
    int SNS_MotorDur    =   -100    ;   // Estimated time to finsh move mS.
    int SNS_MotorDir    =      0    ;   // Motor Direction
    int SNS_CarouselPos =      0    ;   // Encoder Carousel PositionMoving test
    char RTC_I2C_TimeStamp[18] = {"17.05.10 19:03:25"}      ;
    
    int SNS_LastRtn[8]  =   {0,0,0,0,0,0,0,0}   ;   // Last retrurned Value
    int SNS_LastErr[8]  =   {0,0,0,0,0,0,0,0}   ;   // Last retrurned Error
    int SNS_ErrorCnt[8]  =  {0,0,0,0,0,0,0,0}   ;   // Last retrurned Value

    char xRTCTime[18]={"00/00/00 00:00:00\n"}   ;
    char iRTCTime[18]={"00/00/00 00:00:00\n"}   ;


    char    SNS_Buffer[128] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                               0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                               0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                               0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }   ;
    char*   SNS_BufferPtr = SNS_Buffer  ;


int initSensors (void)
{   int stat    = 0 ;   // capture status response from calls
    int errCnt  = 0 ;
//    if (stat == 0)
    {   stat=getI2C2_RTCTime(RTC_I2C_TimeStamp)   ; // verify func calls asnd status
        if(stat != 0)
        {   stat =  DrvrExtRTC + Stage1 + stat ;
            SNS_LastErr[1]=stat ;
            SNS_ErrorCnt[1]++   ;
            errCnt++ ;
        }
        setPIC_RTCTime(RTC_I2C_TimeStamp)         ;
        if(stat != 0)
        {   stat =  DrvrIntRTC + Stage2 + stat ;
            SNS_LastErr[2]=stat ;
            SNS_ErrorCnt[2]++   ;
            errCnt++ ;
        }
    }
    //     if (stat == 0)
    {   stat=initRGB()    ;
        if(stat != 0)
        {   stat =  DrvrRedGrnBlu + Stage3 + stat ;
            SNS_LastErr[3]=stat ;
            SNS_ErrorCnt[3]++   ;
            errCnt++ ;
        }
    }
//   if (stat == 0)
    {   stat=initTHP()    ;
        if(stat != 0)
        {   stat =  DrvrTemHumPrs + Stage4 + stat ;
            SNS_LastErr[4]=stat ;
            SNS_ErrorCnt[4]++   ;
            errCnt++ ;
       }
    }
//    if (stat == 0)
    {   initEncoder()  ;
        if(stat != 0)
        {   stat = DrvrEncoder + Stage5 + stat ;
            SNS_LastErr[5]=stat ;
            SNS_ErrorCnt[5]++   ;
            errCnt++ ;
        }
    }
//    if (stat == 0)
    {   stat=initRadiation()  ;
        if(stat != 0)
        {   stat =  DrvrRadiation + Stage6 + stat ;
            SNS_LastErr[6]=stat ;
            SNS_ErrorCnt[6]++   ;
            errCnt++ ;
        }
    }
    return (errCnt)   ;
}
 
int readSensors (void)
{   int stat    = 0 ;   // capture status response from calls
    int val     = 0 ;   // Encoder position in 1/10ths degrees
    int errCnt  = 0 ;
    {   stat=getI2C2_RTCTime(xRTCTime)         ; // Get External I2C RTC into TimeStamp STring 
        if(stat != 0)
        {   stat =  DrvrExtRTC + Stage1 + stat ;
            SNS_LastErr[1]=stat ;
            SNS_ErrorCnt[1]++   ;
            errCnt++ ;
        }
    }
//    if (stat == 0)
    {
// ** JDP1 **        getTimeStamp(iRTCTime)                   ; // get Internal PIC RTC into timestamp string
        getRTCTimeStamp(iRTCTime)                   ; // get Internal PIC RTC into timestamp string
        if(stat != 0)
        {   stat =  DrvrIntRTC + Stage2 + stat ;
            SNS_LastErr[2]=stat ;
            SNS_ErrorCnt[2]++   ;
            errCnt++ ;
       }
    }
//    if (stat == 0)
    {   stat=readRGB()    ;
        if(stat != 0)
        {   stat =  DrvrRedGrnBlu + Stage3 + stat ;
            SNS_LastErr[3]=stat ;
            SNS_ErrorCnt[3]++   ;
            errCnt++ ;
        }
    }
//    if (stat == 0)
    {   stat=readTHP()    ;
        if(stat != 0)
        {   stat =  DrvrTemHumPrs + Stage4 + stat ;
            SNS_LastErr[4]=stat ;
            SNS_ErrorCnt[4]++   ;
            errCnt++ ;
        }
    }
//    if (stat == 0)
    {   val = readEncoder() ;
        if(stat != 0)
        {   stat =  DrvrEncoder + Stage5 + stat ;
            SNS_LastErr[5]=stat ;
            SNS_ErrorCnt[5]++   ;
            errCnt++ ;
       }
    }
//    if (stat == 0)
    {   stat=readRadiation()  ;
        if(stat != 0)
        {   stat =  DrvrRadiation + Stage6 + stat ;
            SNS_LastErr[6]=stat ;
            SNS_ErrorCnt[6]++   ;
            errCnt++ ;
       }
    }
    return (errCnt)   ;
}

int formatSensors(Byte* Ptr)
{   int BufLen   = 0 ; 
    int BufErr   = BufLenOverflow ;
    BufLen = sprintf(SNS_Buffer, "SNS_Stg: %18s,%18s,|,%li,%li,%li,|,%i,%i,%i,%i,|,%i,%i,|,%i,%i,%i\n",
                    xRTCTime, xRTCTime,
                    SNS_Temperature, SNS_Pressure, SNS_Humidity,
                    SNS_CLR_LIGHT, SNS_RED_LIGHT, SNS_BLU_LIGHT, SNS_GRN_LIGHT, 
                    SNS_EncPeriod, SNS_EncodPos,
                    SNS_VpotCur, SNS_MotorPos, SNS_MotorDur ) ;
    if(BufLen <= 128)
    { BufErr = 0    ;  }    // We Okay, clear built up error code
    
    // SHould we write SAD card from here ???
    return(BufErr)   ;
}

int errorSensors(void)
{   // code goes here to take action(s) to clear errors
    // contents tbd
    //  Not defined function
    if(SNS_LastErr[0] != 0)
    {   // insert recovery attempts here, then clear error
        SNS_LastErr[0]=0 ;
    }
    // External Real time clock
   if(SNS_LastErr[1] != 0)
    {   // insert recovery attempts here, then clear error
        SNS_LastErr[1]=0 ;
    }
    return (0)  ;
    // Internal Real time clock
   if(SNS_LastErr[2] != 0)
    {   // insert recovery attempts here, then clear error
        SNS_LastErr[2]=0 ;
    }
    return (0)  ;
    //  RGB
   if(SNS_LastErr[3] != 0)
    {   // insert recovery attempts here, then clear error
        SNS_LastErr[3]=0 ;
    }
    return (0)  ;
    //  THP
   if(SNS_LastErr[4] != 0)
    {   // insert recovery attempts here, then clear error
        SNS_LastErr[4]=0 ;
    }
    return (0)  ;
    //  Encoder
   if(SNS_LastErr[5] != 0)
    {   // insert recovery attempts here, then clear error
        SNS_LastErr[5]=0 ;
    }
    return (0)  ;
    //  Radiation
   if(SNS_LastErr[6] != 0)
    {   // insert recovery attempts here, then clear error
        SNS_LastErr[6]=0 ;
    }
    // (Not defined)
   if(SNS_LastErr[7] != 0)
    {   // insert recovery attempts here, then clear error
        SNS_LastErr[7]=0 ;
    }
    return (0)  ;
}
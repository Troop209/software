/* 
 * File:   normain.c
 * Author: Norman McFarland
 *
 * Created on February 27, 2016, 1:10 PM
 */

// #include <stdio.h>
// #include <stdlib.h>

#include "P24FJ256GB106.h"    // included in system.h

#include "system.h"
#include "nesi.h"

#include "Switch_Align_Home.h"
#include "PIC24_Peripherals.h"
#include "PIC24_I2C2_R7.h"
#include "RealTimeClock.h"
#include "RedGreenBlue.h"
#include "Gyroscope.h"
#include "TempPresHumd.h"
#include "I2C2_dateTime.h"
#include "InertMeasUnit.h"

char RTC_I2C_TimeStampData[18] = {"16.05.03:17:03:25"}      ;
String tstamp={"00-00-00 00:00:00"}   ;

int main(int argc, char** argv) 
{   //
    int select  = 102;
    int Tdelay  = 10      ;
    long Mdelay  = 10000      ;
   // Switch statement control variable
    int cnt     = 30000   ;
    int dutcycl = 24000   ;
    int mot     = 5000    ;
    int period  = 3125    ;
    int pos      = 0      ;
    int position = 16000  ; 
    int stat     = 0      ;   // return status
    int valu     = 0      ;   //returned value
    int catch   = 500     ;
    char IMU_buf[96]={0x55} ;
    int  IMU_val[6]         ; // IMU 3 axis value
    long int nanoseconds=0  ;
    long int dutcyc =   0   ;
    
    int PWM_Positions[10] = {16000,18000,20000,22000,24000,26000,28000,30000,32000,16000}    ;
 
// Initialization begins here
    
    system.init()   ;
    dateTime.init()   ;
 _TRISE6 = 0;               // configure port as output
 _RE6 = 1; // set the output (active high)  Turn on 6Volt supply 
 // Init I21C2    
// Init Servo motor
    PWM9_init()       ;
    SET_PWM9_Period(period)       ;
    SET_PWM9_DutyCycle(dutcycl)   ;
// Init A2D Channel 10- motor current
//    A2D_c10_init()              ; 
    Analog_init()    ;
     
    // i2c2_TalkToDevice(RTC_ADDR, RTC_SET_LENGTH, STG_SET_TIME, 0, 0)              ;
    i2c2_init(75)       ;     // I2C2_KHz
    i2c2_reset_bus()    ;
           
  while (cnt>0)
  { switch  (select)
    {  case 0  :
         while (mot>0)
        { pos=0   ;
          while (pos <9)
          { position=PWM_Positions[pos];
            SET_PWM9_DutyCycle(position)   ;
            delay(Mdelay)  ;
            pos++       ;
          }
          --mot ;
         }
        break   ;
       case 1   :
        valu = Read_Switch_Align()   ;
        break   ;
       case 2    :
        valu = Read_Switch_Home()    ;
        break   ;
       case 3    :
         nanoseconds= RSP78_init ()    ;
        break   ;
       case 4    :
        dutcyc = RSP78_Read_Sig_Period()        ;
        break   ;
       case 5    :
        valu = A2D_c10_read()    ;
        break   ;
       case 6    :
        valu = A2D_c10_sample(5,1)  ;
        break   ;
       case 7    :
        SET_PWM9_Period( period)       ;
        break   ;
       case 8    :
        SET_PWM9_DutyCycle(dutcycl)   ;
       break   ;
       case 9    :
       break       ;
 // *********************************************
 //    Camera 1 Testing
 //    ******************************************   

        case 10    :
         initializeCam1()   ;
         break              ;
        case 11    :
         setOutputCam1(0)   ;
         break              ;
        case 12    :
         setOutputCam1(1)   ;
         break             ;
        case 13    :
         setOffCam1()       ;
         break              ;
        case 14    :
         setOnCam1()        ;
         break              ;
     
// *********************************************
//     Camera 2 Testing
//     *****************************************      
        case 20    :
         initializeCam2()   ;
         break              ;
        case 21    :
         setOutputCam2(0)   ;
         break              ;
        case 22    :
         setOutputCam2(1)   ;
         break             ;
        case 23    :
         setOffCam2()       ;
         break              ;
        case 24    :
         setOnCam2()        ;
         break              ;
     
// *********************************************
//     LED 1 Testing
// *********************************************       
        case 30    :
         initializeLED1()   ;
         break              ;
        case 31    :
         setOutputLED1(0)   ;
         break              ;
        case 32    :
         setOutputLED1(1)   ;
         break             ;
        case 33    :
         setOffLED1()       ;
         break              ;
        case 34    :
         setOnLED1()        ;
         break              ;
     
// *********************************************
//     LED2 Testing
// *********************************************       
        case 40    :
         initializeLED2()   ;
         break              ;
        case 41    :
         setOutputLED2(0)   ;
         break              ;
        case 42    :
         setOutputLED2(1)   ;
         break             ;
        case 43    :
         setOffLED2()       ;
         break              ;
        case 44    :
         setOnLED2()        ;
         break              ;
     
// *********************************************
//     Servo 1 Testing
// *********************************************      
        case 50    :
         initializeServo()   ;
         break              ;
        case 51    :
         setOutputServo(0)   ;
         break              ;
        case 52    :
         setOutputServo(1)   ;
         break             ;
        case 53    :
         setOffServo()       ;
         break              ;
        case 54    :
         setOnServo()        ;
         break              ;
    
// *********************************************
//    PING I2C2 Device Testing
// *********************************************      
        case 60    :
         i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr)  ;       // Clear Diagn ostic Countrers
         break             ;
        case 61    :
         stat=i2c2_TalkToDevice(RTC_ADDR,  0, NullPtr,  0, NullPtr)                ; 
         break              ;
        case  62    :
         setI2C2_RTCTime(RTC_I2C_TimeStampData)   ;     // RGB Gesture
         
         select = 63   ;
         break             ;
        case  63    :
         getI2C2_RTCTime(RTC_I2C_TimeStampData)  ;     //Temp Humidity Pressure
         break              ;
        case  64    :
         stat=i2c2_TalkToDevice(0x6B,  0, NullPtr,  0, NullPtr)                ; 
         break              ;
        case  65    :
         stat=i2c2_TalkToDevice(RTC_ADDR,  0, NullPtr,  0, NullPtr)                ;
         stat=i2c2_TalkToDevice(0x39,  0, NullPtr,  0, NullPtr)                ; 
         stat=i2c2_TalkToDevice(0x77,  0, NullPtr,  0, NullPtr)                ; 
         stat=i2c2_TalkToDevice(0x1D,  0, NullPtr,  0, NullPtr)                ; 
         stat=i2c2_TalkToDevice(0x6B,  0, NullPtr,  0, NullPtr)                ; 
         break                   ;
        case 66     :
         // dt=datetime.get      ;
         break   ;
        case  69    :
         stat=i2c2_TalkToDevice(RTC_ADDR, 0, NullPtr,  0, NullPtr)            ;      // Real Time Clock Set Harmon Time
         stat=i2c2_TalkToDevice(0x39, 0, NullPtr,  0, NullPtr)                ;      // RGBC
         stat=i2c2_TalkToDevice(0x77, 0, NullPtr,  0, NullPtr)                ;      // THP
         stat=i2c2_TalkToDevice(0x1D, 0, NullPtr,  0, NullPtr)                ;      // Accel/Magne
         stat=i2c2_TalkToDevice(0x6B, 0, NullPtr,  0, NullPtr)                ;      // Gyro
         break              ;
//  *********************************************
//    RTC I2C2 Test both Real Time Clocks
// *********************************************      
        case 70     :
         // Clear Diagnostic Countrers
         stat=i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr) ;       
         delay(1)           ;
         break              ;
        case  71    :
         // Ping using talk to device
         stat=i2c2_TalkToDevice(RTC_ADDR,  0, NullPtr,  0, NullPtr)                ; 
         delay(1)           ;
        break              ;
        case  72    :
         stat=i2c2_TalkToDevice(RTC_ADDR, RTC_GET_LENGTH, RTC_SET_TIME, RTC_RSP_LENGTH, RTC_GET_TIME)   ; // get I2C RTC BCD string
         delay(1)           ;
         break              ;
        case  73    :
         stat=i2c2_TalkToDevice(RTC_ADDR, RTC_SET_LENGTH, RTC_SET_TIME,  0, NullPtr)                    ; // set I2C RTC BCD string
         delay(1)           ;
        break              ;
        case  74    :
         stat=getI2C2_RTCTime(RTC_I2C_TimeStampData)         ; // Get I2C RTC into TimeStamp STring 
         delay(1)           ;
        break              ;
        case  75    :
         stat = setI2C2_RTCTime(RTC_I2C_TimeStampData)  ; // Set I2c RTC from TimeStamp STring
         delay(1)           ;
         break              ;
        case  76    :
         getTimeStamp(RTC_I2C_TimeStampData)                   ; // get PIC RTC into timestamp string
         delay(1)           ;
        break              ;
        case  77    :
         setPIC_RTCTime(RTC_I2C_TimeStampData)         ; // set PIC RTC from timestamp string
         delay(1)           ;
         break              ;
        case  78    :
         delay(1)           ;
         break              ;
        case  79    :
         stat=getI2C2_RTCTime(RTC_I2C_TimeStampData)        ; 
         setPIC_RTCTime(RTC_I2C_TimeStampData)         ;
         delay(1)           ;
       break              ;
 // *********************************************
//    GYR I2C2 Decive Testing
// *********************************************   
        case 80    :
         // Clear the Diagnostic buffer
         stat=i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr)  ;               // Clear Diagn ostic Countrers
         delay (1)  ;
         break              ;
        case 81    :
         stat=enableIMU()        ;
         delay (1)  ;
         break              ;
        case 82    :
         stat= dumpIMU(IMU_buf) ;
          delay (1)  ;
        break             ;
        case 83    :
         stat= readACC(IMU_val)    ; 
         delay (1)  ;
        break              ;
        case 84    :
         stat=readMAG(IMU_val)    ;
         delay (1)  ;
         break              ;
        case 85    :
         stat=readGYR(IMU_val)     ;
         delay (1)  ;
         break              ;
        case 86    :
         stat=i2c2_TalkToDevice(ACC_Addr, 0, NullPtr, 0, NullPtr)  ;
         delay (1)  ;
         break     ;
       case 87    :
         stat=gyro3_main(argc, argv)  ;
         delay (1)  ;
        break     ;
        case 88    :
         stat=AMG5_main(argc, argv)   ;
         delay (1)  ;
         break     ;
        case 89    :
        delay (1)  ;         
        break      ;
// *********************************************
//    RGB I2C2 Decive Testing
// *********************************************      
        case 90    :
         stat=i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr) ;              // Clear Diagn ostic Countrers
         RGB_Configure1 ()       ;
         select=94  ;   // read back values
         break              ;
        case 91    :
         RGB_Configure2 ()       ;
         break              ;
        case 92    :
         RGB_Configure3 ()       ;
         break             ;
        case 93    :
         RGB_ReadStatus ()       ;
         break              ;
        case 94    :
         RGB_ReadValues ()      ;
         break              ;
        case 95    :
         RGB_ClearIrqs ()       ;
         break              ;
        case 99    :
         stat=RGB_Configure1 ()       ;
         stat=RGB_ReadStatus ()       ;
         stat=RGB_ReadValues ()      ;
         stat=RGB_ReadStatus ()       ;
         stat=RGB_ClearIrqs ()       ;
         break              ;
// *********************************************
//    THP I2C2 Device Testing
// *********************************************      
        case 100    :
         stat= i2c2_TalkToDevice(THP_Addr,0, NullPtr, 0, NullPtr) ; // ping device
         break              ;
        case 101    :
         // i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr) ; 
         stat = THP_Configure1 ()       ;
         // select=105  ;   // read back values
         break              ;
        case 102    :
         i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr) ; 
         stat = THP_Configure2 ()       ;
         select=106  ;   // read back values
         break              ;
        case 103    :
         stat = THP_Configure3 ()       ;
         break             ;
        case 104    :
        // stat = THP_ReadStatus ()       ;     // part of read values
        // select=104  ;   // read back values
        break              ;
       case 105    :
         // stat = THP_Diag_RdIrqs ()       ;       
        THP_Init ()    ;
        break              ;
        case 106    :
         stat = THP_Read_Values ()      ;
         break              ;
       case 107    :
         // stat = THP_Diag_RdIrqs ()       ;
         break      ;
       case 108    :
         // stat = THP_Init () ;
         // stat = THP_Diag_RdIrqs ()       ;
         break      ;
        case 109    :
         stat = THP_Configure1 ()       ;
         // THP_ReadStatus ()       ;
         stat = THP_Read_Values ()      ;
         // stat = THP_ReadStatus ()       ;
         // THP_Diag_RdIrqs ()       ;
         break    ;
      default:
          delay(10) ;
    }       // end of switch
// *********************************************
//     SWITCH ENDS
// *********************************************
    if(cnt % catch==0)
     { delay(10)    ;
     }
    delay (Tdelay) ;
    cnt--       ;
  }     // end of while
  return (0);
}
;
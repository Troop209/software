/* 
 * File:   HdwrTest.c
 * Author: Norman McFarland
 *
 * Created on Nov 19, 2016, 3:10 PM
 * 
 * Revision Notes
 * Jan 17, 2017
 * 1. Add Open file 3, close file 3, write to file 3 (Multiples)
 * 2. Add 'loopDelay' condition to add time between sets of motor moves
 * 3. 
 */

#include "P24FJ256GB106.h"    // included in system.h

#include "system.h"
#include "nesi.h"

#include "PortBitDrivers.h"
#include "SensorDrivers.h"
#include "RadiationSensorDriver.h"
#include "I2CDrivers.h"
#include "RedGreenBlueDrivers.h"
#include "TempHumidPresDrivers.h"
#include "dateTime_RTC.h"
#include "Analog2Digital.h"
#include "Encoder.h"
#include "NVMDrivers.h"
#include "camera.h"
#include "../core/file.h"
#include "multiMotorDrivers.h"
#include "RadiationSensorDriver.h"
#include "AltTemp.h"
#include "OptoDrivers.h"
//07/09 - KYu commented out since Norm did not send send the file
//#include "VirtualRealTimeClock.h"
#include "stdio.h"



/** Attempt to re-use Camera buffer rather than waste more memory
 *
 * BUFFER_SIZE Information
 *
 * BUFFER_SIZE defines the size of buffers used throughout the serial module.
 */
// NRM // #define RX_BUFFER_SIZE    0x0800

//
// * RX_buffer Information
// *
// * The RX_buffer holds bytes that have been received by the UART module. This
// * global circular buffer is serviced in the background via interrupts. The
// * buffer is indexed via RX_head and RX_next.
// NRM // static Byte RX_buffer[RX_BUFFER_SIZE] = {0};
// NRM // static volatile Uint16 RX_head = 0;
// NRM // static volatile Uint16 RX_next = 0;

    int enableMotor       = 3 ;  //  0= No; 1=Absolute; 2=Relative; 3=sequence of 8 4= seq of 17, 5=180 calibration point
    int pingXRTC          = 0 ;  //  0= No; 1=Yes
    int pingRGB           = 0 ;  //  0= No; 1=Yes
    int pingTHP           = 0 ;  //  0= No; 1=Yes
    int readAllSensors    = 0 ;  //  0= No; 1=Yes
    int readAltLightSensor= 0 ;  //  0= No; 1=Yes
    int readAltTempSensor = 0 ;  //  0= No; 1=Yes
    int readEncoderValue  = 1 ;  //  0= No; 1=Yes
    int calibEncoderValue = 0 ;  //  0= No; 1=Yes
    int readIRTCValue     = 0 ;  //  0= No; 1=Yes
    int readNVMValue      = 0 ;  //  0= No; 1=Yes
    int readOptoSensor    = 0 ;  //  0= No; 1=Yes
    int readRadSensor     = 0 ;  //  0= No; 1=Yes
    int readRGBSensor     = 0 ;  //  0= No; 1=Yes
    int readTHPSensor     = 0 ;  //  0= No; 1=Yes
    int readVRTCValue     = 0 ;  //  0= No; 1=Yes
    int readXRTCValue     = 0 ;  //  0= No; 1=Yes
    int takePicture1      = 0 ;  //  0= No; 1=Yes
    int takePicture2      = 0 ;  //  0= No; 1=Yes
    int toggleCAM1        = 0 ;  //  0= No; 1=Yes
    int toggleCAM2        = 0 ;  //  0= No; 1=Yes
    int toggleLED1        = 0 ;  //  0= No; 1=Yes
    int toggleLED2        = 0 ;  //  0= No; 1=Yes
    int usbConnect        = 0 ;  //  0= No; 1=Yes
    int writeIRTC         = 0 ;  //  0= No; 1=Yes
    int writeVRTC         = 0 ;  //  0= No; 1=Yes
    int writeXRTC         = 0 ;  //  0= No; 1=Yes
    int writeSensors      = 0 ;  //  0= No; 1=Yes
    int writeTempHumPres  = 0 ;  //  0= No; 1=Yes
    int delay05Sec        = 0 ;  //  0= No; 1=Yes, delay 5 secs
    int delay15Sec        = 0 ;  //  0= No; 1=Yes, delay 15 secs
    int delay30Sec        = 0 ;  //  0= No; 1=Yes, delay 30 secs

    int stat              = 0 ;   // Return status of HW related functions
    int THPtempADDR       = 0 ;
    
    
void HdwrTest(void)
{   // $$$ Conditional operation s variables
    extern char RTC_I2C_TimeStamp[18]    ;
    extern char xRTCTime[18] ;   
    extern int  SNS_AltTemp  ;
    extern int  SNS_AltLight ;
    extern int  moveStatus ; 
    int speed           =  7 ;
    int angle           =  0 ;
    
    char filename[40] = {0};
    
  // Initialization begins here
    // system.init()   ;
    i2c2_init(75)       ;       // Initialize I2C2 and set Baud Rate
    i2c2_reset_bus()    ;       // Reset I2C Bus/Network      
    dateTime.init()     ;       // Init internal RTeal Time Clock   $$$ May need to mod this $$$
    initA2D()           ;       // Configure Q1-Q4 Analog Inputs. Q3 Currently used for VMot check.
    stat = initSensors()       ;       // set up sensors // returns Error count of # sensors with errors. 0 Desired
    stat=i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr) ;      // clear I2C2 diagnostic counters   
    initStepper()       ;
    // initEncoder()       ;
    homeCarousel()    ;
    // Initialization done here

    delay(1)   ; // Breakpoint here to adjust conditional variables before running loop
 
  while (1)
  { delay(1)    // Breakpoint here to adjust conditional variables while running loop
    if ( readTHPSensor        == 1)   //  0= No; 1=Yes    
    {   stat = readTHP() ; //   
    }
    if ( readRGBSensor        == 1)   //  0= No; 1=Yes    
    {   stat = readRGB() ; //    
    }
    if ( readRadSensor        == 1)   //  0= No; 1=Yes    
    {   // stat = readRadiationIrq() ; //    
        stat = readRadiation() ; //    
    }
    if ( readAltTempSensor      == 1)   //  0= No; 1=Yes    
    {   SNS_AltTemp=readAltTemp()  ; //    
    }
    if ( readAltLightSensor     == 1)   //  0= No; 1=Yes    
    {   SNS_AltLight=readAltLight()  ; //    
    }
    if ( readEncoderValue    == 1)   //  0= No; 1=Yes
    {   //
      // stat = readEncoderIrq()  ;
      stat = readEncoder()  ;       // returns Error count of # sensors with errors. 0 Desired
    }
// 07/09 KYu commented out waiting on Norm file
// --------------------------------------------------  
//    if ( readXRTCValue       == 1)   //  0= No; 1=Yes    
//    {   readI2CTimeStamp(RTC_I2C_TimeStamp)  ; //    
//    }
//    if ( readIRTCValue       == 1)   //  0= No; 1=Yes    
//    {   readPICTimeStamp(RTC_I2C_TimeStamp)  ;
//    }
//    if ( readVRTCValue       == 1)   //  0= No; 1=Yes    
//    {   readVirtTimeStamp()    ;   // get next vRTC value. returns 0 status    
//    }
// --------------------------------------------------  
    if ( readOptoSensor       == 1)   //  0= No; 1=Yes    
    {   setOutputServo(1)   ;   // Enable Opto LED's
        stat =  readOptos() ; //
        setOutputServo(0)   ;   // Disable Opto LED's
    }
    if ( pingXRTC        == 1)   //  0= No; 1=Yes    
    {       stat=i2c2_TalkToDevice(0x68, 0, NullPtr, 0, NullPtr) ;      // ping RTC
    }
    if ( pingRGB        == 1)   //  0= No; 1=Yes    
    {       stat=i2c2_TalkToDevice(0X39, 0, NullPtr, 0, NullPtr) ;      // ping RTC
    }
    if ( pingTHP        == 1)   //  0= No; 1=Yes    
    {       stat=i2c2_TalkToDevice(0X77, 0, NullPtr, 0, NullPtr) ;      // ping RTC NRm aDDRESS chANGE 5/30/17 FROM 0X77
    }
    if ( readNVMValue        == 1)   //  0= No; 1=Yes    
    {   stat =  testNVM() ; //    
    }
// 07/09 KYu commented out waiting on Norm file
// --------------------------------------------------  
//    if ( writeXRTC      == 1)   //  0= No; 1=Yes    
//    {   writeI2CTimeStamp(xRTCTime)  ;   
//    }
//    if ( writeIRTC      == 1)          // Read xRTC
//    {   writePICTimeStamp(xRTCTime)   ;
//    }
//    if ( writeVRTC      == 1)   //  0= No; 1=Yes    
//    {   writeVirtTimeStamp(xRTCTime)  ; // set virtual clock beginning date and time
//    }
// --------------------------------------------------  
    if ( toggleLED1     == 1)   //  0= No; 1=Yes    
    {   setOutputLED1(1)   ; 

        delay(100) ; //
        setOutputLED1(0)   ; 
        delay(100) ; //
        setOutputLED1(1)   ; 
        delay(100) ; //
        setOutputLED1(0)   ; 
        delay(100) ; //        
    }
    if ( toggleCAM1     == 1)   //  0= No; 1=Yes    
    {   setOutputCam1(1)   ; 
        delay(100) ; //
        setOutputCam1(0)   ; 
        delay(100) ; //
        setOutputCam1(1)   ; 
        delay(100) ; //
        setOutputCam1(0)   ; 
        delay(100) ; //
    }
    if ( toggleLED2     == 1)   //  0= No; 1=Yes    
    {   setOutputLED2(1)   ; 
        delay(100) ; //
        setOutputLED2(0)   ; 
        delay(100) ; //
        setOutputLED2(1)   ; 
        delay(100) ; //
        setOutputLED2(0)   ; 
        delay(100) ; //       
    }
    if ( toggleCAM2     == 1)   //  0= No; 1=Yes    
    {   setOutputCam2(1)   ; 
        delay(100) ; //
        setOutputCam2(0)   ; 
        delay(100) ; //
        setOutputCam2(1)   ; 
        delay(100) ; //
        setOutputCam2(0)   ; 
        delay(100) ; //    
    }
    if ( readAllSensors     == 1)   //  0= No; 1=Yes
    {   //
      stat = readSensors()  ;       // returns Error count of # sensors with errors. 0 Desired
    }
    if ( writeSensors    == 1)      //  0= No; 1=Yes
    {   //
       stat = formatSensors(RX_buffer)  ;   // $$$ returns LENGTH of line if <128
       // $$$ Add code to write the buffer to output file on SD card    
      
    // $$$    file3.write(Byte *RX_buffer, stat);
    }
// 07/09 KYu commented out waiting on Norm file
// --------------------------------------------------  
//    if ( writeTempHumPres== 1)        //  0= No; 1=Yes
//    {   
//        diagRecord()    ; // $$$ Need to uncomment this code in TempHumPresDrives, uncomment '#include file.h'
//    }
// --------------------------------------------------  
    if ( enableMotor     >= 1)        //  0= No; 1=Toggle; 2=Sequence; 3=180 calibration point
    {   stat=checkCarousel(enableMotor, angle, speed)   ;
        // if (stat == functionSUCCESS)
        if (stat == 0)  // functionSUCCESS
        { stat=moveCarousel(enableMotor, angle, speed)   ;
        delay(1000); //Just to test motor JM
        }
    }
   if ( takePicture1    == 1)        //  0= No; 1=Yes
    {   //
        if(moveStatus==0)
        {
            setOutputLED1(1)   ; 
            camera.getPix(filename);
            setOutputLED1(0)   ;
        }
    }
    if ( takePicture2    == 1)        //  0= No; 1=Yes
    {   //
        if(moveStatus==0)
        {
        setOutputLED2(1)   ;
        camera2.getPix(filename);
        setOutputLED2(0)   ;
        }
    }
    if ( usbConnect    == 1)          //  0= No; 1=Yes
    {   //
       testNVM()    ;
       // $$$ Insert function call to execute USB Connect function  
    }
    if ( delay05Sec  == 1)
    {   delay(5000) ;
    }
    if ( delay15Sec  == 1)   
    {   delay(15000) ;
    }
    if ( delay30Sec   ==1) 
    {   delay(30000) ;
    }
    delay(1)    // end breakpoint line
  }
  // file3.close()   ;          

}

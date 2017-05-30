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

    int readAllSensors    = 0 ;  //  0= No; 1=Yes
    int readTHPSensor     = 1 ;  //  0= No; 1=Yes
    int readRGBSensor     = 0 ;  //  0= No; 1=Yes
    int readRadSensor     = 0 ;  //  0= No; 1=Yes
    int readAltTempSensor = 0 ;  //  0= No; 1=Yes
    int readAltLightSensor= 0 ;  //  0= No; 1=Yes
    int readXRTCValue     = 0 ;  //  0= No; 1=Yes
    int readIRTCValue     = 0 ;  //  0= No; 1=Yes
    int readVRTCValue     = 0 ;  //  0= No; 1=Yes
    int readOptoSensor    = 0 ;  //  0= No; 1=Yes
    int readNVMValue      = 0 ;  //  0= No; 1=Yes
    int pingXRTC          = 0 ;  //  0= No; 1=Yes
    int pingRGB           = 0 ;  //  0= No; 1=Yes
    int pingTHP           = 1 ;  //  0= No; 1=Yes
    int writeXRTC         = 0 ;  //  0= No; 1=Yes
    int writeIRTC         = 0 ;  //  0= No; 1=Yes
    int toggleLED1        = 0 ;  //  0= No; 1=Yes
    int toggleLED2        = 0 ;  //  0= No; 1=Yes
    int toggleCAM1        = 0 ;  //  0= No; 1=Yes
    int toggleCAM2        = 0 ;  //  0= No; 1=Yes
    int readEncoderValue  = 0 ;  //  0= No; 1=Yes
    int writeSensors      = 0 ;  //  0= No; 1=Yes
    int writeTempHumPres  = 0 ;  //  0= No; 1=Yes
    int enableMotor       = 3 ;  //  0= No; 1=Absolute; 2=Relative; 3=sequence of 8 4= seq of 17, 5=180 calibration point
    int takePicture1      = 0 ;  //  0= No; 1=Yes
    int takePicture2      = 0 ;  //  0= No; 1=Yes
    int usbConnect        = 0 ;  //  0= No; 1=Yes
    int delay05Sec        = 0 ;  //  0= No; 1=Yes, delay 5 secs
    int delay15Sec        = 0 ;  //  0= No; 1=Yes, delay 15 secs
    int delay30Sec        = 0 ;  //  0= No; 1=Yes, delay 30 secs
    int HappypathInit     = 0 ;  //  0= No; 1=Yes(Decision is use happy path init)
    int stat              = 0 ;   // Return status of HW related functions
    
void HdwrTest(void)
{   // $$$ Conditional operation s variables
    extern char RTC_I2C_TimeStamp[18]    ;
    extern char xRTCTime[18] ;   
    extern char iRTCTime[18] ;
    extern int  SNS_AltTemp  ;
    extern int  SNS_AltLight ;
    int speed           =  7 ;
    int angle           =  0 ;
    
    char filename[40] = "Default";
    
  // Initialization begins here
    if(HappypathInit == 0)  //Decide to use happy path init or not
    {
    // system.init()   ;
    i2c2_init(75)       ;       // Initialize I2C2 and set Baud Rate
    i2c2_reset_bus()    ;       // Reset I2C Bus/Network      
    dateTime.init()     ;       // Init internal RTeal Time Clock   $$$ May need to mod this $$$
    initA2D()           ;       // Configure Q1-Q4 Analog Inputs. Q3 Currently used for VMot check.
    // stat = initSensors()       ;       // set up sensors // returns Error count of # sensors with errors. 0 Desired
    stat=i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr) ;      // clear I2C2 diagnostic counters   
    initStepper()          ;  
    // stat = calibrateEncoder()      ;
    // stat = monitorEncoder(16)    ;
  // Initialization done here
    }
    else
    {
        init_experiment();
    }
    delay(1)   ; // Breakpoint here to adjust conditional variables before running loop
 
  while (1)
  { 
    delay(1)    // Breakpoint here to adjust conditional variables while running loop
    if ( readTHPSensor        == 1)   //  0= No; 1=Yes    
    {   stat = readTHP() ; //   
    nop();
    nop();
    nop();
    }
    if ( readRGBSensor        == 1)   //  0= No; 1=Yes    
    {   stat = readRGB() ; //    
    }
    if ( readRadSensor        == 1)   //  0= No; 1=Yes    
    {   stat = readRadiationIrq() ; //    
        stat = readRadiation() ; //    
    }
    if ( readAltTempSensor      == 1)   //  0= No; 1=Yes    
    {   SNS_AltTemp=readAltTemp()  ; //    
    }
    if ( readAltLightSensor     == 1)   //  0= No; 1=Yes    
    {   SNS_AltLight=readAltLight()  ; //    
    }
    if ( readEncoderValue   == 0 ) ;  //  0= No; 1=Yes
    {   //
      // stat = readEncoderIrq()  ;
      stat = readEncoder()  ;       // returns Error count of # sensors with errors. 0 Desired
    }
    if ( readXRTCValue       == 1)   //  0= No; 1=Yes    
    {   stat =  getI2C2_RTCTime(xRTCTime)  ; //    
    }
    if ( readIRTCValue       == 1)   //  0= No; 1=Yes    
    {   getRTCTimeStamp(iRTCTime) ; //    
    }
    if ( readVRTCValue       == 1)   //  0= No; 1=Yes    
    {   // stat =  ; //    
    }
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
    {       stat=i2c2_TalkToDevice(0X77, 0, NullPtr, 0, NullPtr) ;      // ping RTC
    }
    if ( readNVMValue        == 1)   //  0= No; 1=Yes    
    {
        stat =  testNVM() ; //    
    }
    if ( writeXRTC      == 1)   //  0= No; 1=Yes    
    {  // stat =  ; //    
    }
    if ( writeIRTC      == 1)   //  0= No; 1=Yes    
    {   stat =  getI2C2_RTCTime(xRTCTime)  ;        // Read xRTC
        setPIC_RTCTime(RTC_I2C_TimeStamp);  // wRITE IRTC   
    }
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
    if ( writeTempHumPres== 1)        //  0= No; 1=Yes
    {   
        //
            }
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
        setOutputLED1(1)   ; 
        sprintf(filename, "%s-%s-cam1.jpg", "Default", dateTime.getStamp());
        fixcolons(filename);
        camera.getPix(filename);
        delay(1000);
        setOutputLED1(0)   ;
    }
    if ( takePicture2    == 1)        //  0= No; 1=Yes
    {   //
        setOutputLED2(1)   ;
        sprintf(filename, "%s-%s-cam2.jpg", "Default", dateTime.getStamp());
        fixcolons(filename);
        camera2.getPix(filename);
        delay(1000);
        setOutputLED2(0)   ;
    }
    if ( usbConnect    == 1)          //  0= No; 1=Yes
    {   //
       usb.connect();
       delay(100000);
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

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

SDConfig config;

static char logLabel[200];
static char buf[50];

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

    int moveStatus      =  1 ;
// stores timestamped picture
Boolean takePicture(void)
{
    char filename[40] = {0};
    
    // the ':' character is an invalid character, so it needs to be changed
    sprintf(filename, "%s-%s-cam1.jpg", config.label, dateTime.getStamp());
    fixcolons(filename);
    camera.getPix(filename);

    delay(1000);

    sprintf(filename, "%s-%s-cam2.jpg", config.label, dateTime.getStamp());
    fixcolons(filename);
    camera2.getPix(filename);

    return 0;
}

void fixcolons(char *temp)
{
    while(*temp)
    {
        if(*temp == ':')
            *temp = '.';
        
        ++temp;
    }
}

void HdwrTest(void)
{   // $$$ Conbditional operation s variables
    int readAllSensors  = 0 ;  //  0= No; 1=Yes
    int readEncoderOnly = 0 ;  //  0= No; 1=Yes
    int writeSensors    = 0 ;  //  0= No; 1=Yes
    int writeTempHumPres= 0 ;  //  0= No; 1=Yes
    int enableMotor     = 3 ;  //  0= No; 1=Absolute; 2=Relative; 3=sequence of 8 4= seq of 17, 5=180 calibration point
    int takePicture1    = 0 ;  //  0= No; 1=Yes
    int takePicture2    = 0 ;  //  0= No; 1=Yes
    int usbConnect      = 0 ;  //  0= No; 1=Yes
    int delay05Sec      = 0 ;  //  0= No; 1=Yes, delay 5 secs
    int delay15Sec      = 0 ;  //  0= No; 1=Yes, delay 15 secs
    int delay30Sec      = 0 ;  //  0= No; 1=Yes, delay 30 secs
    
    int speed           =  7 ;
    int angle           =  0 ;
    
    int stat            =  0 ;   // Return status of HW related functions

    char filename[40] = {0};
    char FileNamePrefix[13]={"HdwrTest01: "}    ;
    int  encPos          = 0 ;
    
    extern unsigned int SNS_EncodPos ;
    
    // Initialization begins here
    system.init()   ;
 
    // $$$ init kernal peripherals here
    // $$$ ...
    // $$$ Finish Kernal periferal init here
    
    i2c2_init(75)       ;       // Initialize I2C2 and set Baud Rate
    i2c2_reset_bus()    ;       // Reset I2C Bus/Network
           
    dateTime.init()     ;       // Init internal RTeal Time Clock   $$$ May need to mod this $$$
    
    setOn6Volt()        ;        // Turn on 6V Supply (for now)     
 
    initA2D()           ;       // Configure Q1-Q4 Analog Inputs. Q3 Currently used for VMot check.
    
    stat = initSensors()       ;       // set up sensors // returns Error count of # sensors with errors. 0 Desired
    
    stat=i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr) ;      // clear I2C2 diagnostic counters   

    initStepper()          ;  
    
    // file3.open("TestData.csv");          // $$$ told 'file3' defined in file.h inclusions
    // stat = calibrateEncoder()      ;
    
    // stat = monitorEncoder(16)    ;       // NRM //
    
    delay(1)    // 'before' breakpoint line
 // $$$ Start Main Loop

  while (1)
  { delay(1)    // 'during' breakpoint line
    if ( readAllSensors     == 1)   //  0= No; 1=Yes
    {   //
      stat = readSensors()  ;       // returns Error count of # sensors with errors. 0 Desired
    }
    if ( readEncoderOnly     == 1)   //  0= No; 1=Yes
    {   //
      // encPos = readEncoder()  ;        // returns -1 (no signal) or 5Full scale as 0-4095
    }
    if ( writeSensors    == 1)      //  0= No; 1=Yes
    {   //
       stat = formatSensors(RX_buffer)  ;   // $$$ returns LENGTH of line if <128
       // $$$ Add code to write the buffer to output file on SD card    
      
    // $$$    file3.write(Byte *RX_buffer, stat);
    }
    if ( writeTempHumPres== 1)        //  0= No; 1=Yes
    {   //
        diagRecord()    ; // $$$ Need to uncomment this code in TempHumPresDrives, uncomment '#include file.h'
    }
    if ( enableMotor     >= 1)        //  0= No; 1=Toggle; 2=Sequence; 3=180 calibration point
    {   stat=checkCarousel(enableMotor, angle, speed)   ;
        // if (stat == functionSUCCESS)
        if (stat == 0)  // functionSUCCESS
        { stat=moveCarousel(enableMotor, angle, speed)   ;
        }
    }
   if ( takePicture1    == 1)        //  0= No; 1=Yes
    {   //
        setOutputLED1(1)   ;
    
        // the ':' character is an invalid character, so it needs to be changed
        sprintf(filename, "%s-%s-cam1.jpg", FileNamePrefix, dateTime.getStamp());
        fixcolons(filename);
        camera.getPix(filename);

        delay(1000);

        setOutputLED1(0)   ;
        delay (100)        ;
    }
    if ( takePicture2    == 1)        //  0= No; 1=Yes
    {   //
        setOutputLED2(1)   ;

        sprintf(filename, "%s-%s-cam2.jpg", FileNamePrefix, dateTime.getStamp());
        fixcolons(filename);
        camera2.getPix(filename);
        delay (2000)        ;
 
        setOutputLED2(0)   ;
        delay (100)    ;
    }
    if ( usbConnect    == 1)          //  0= No; 1=Yes
    {   //
       TestNVM()    ;
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

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
#include "PIC24_I2C2_R5.h"
//#include "Real Time Clock.h"


int main(int argc, char** argv) 
{   //
    int cnt =   5000  ;
    int mot =   5000  ;
    int pos =    0          ;
    int position = 16000    ; 
    int stat    = 0 ;   // return status
    int select  = 60 ;   // Switch statement control variable
    int valu    = 0 ;   //returned value
 
    long int nanoseconds=0  ;
    long int dutcyc =   0   ;
    
    int PWM_Positions[18] = {16000,17000,18000,19000,20000,21000,22000,23000,24000,
                         25000,26000,27000,28000,29000,30000,31000,32000,16000}    ;
 
// Initialization begins here
    
    system.init()   ;
      
 // Init I21C2    
    i2c2_init(75)       ;     // I2C2_KHz
    i2c2_reset_bus()    ;
// Init Servo motor
    PWM9_init()       ;
    SET_PWM9_Period( 3124)       ;
    SET_PWM9_DutyCycle( 24000)   ;
// Init A2D Channel 10- motor current
//    A2D_c10_init()              ; 
     
    // i2c2_TalkToDevice(0x68, RTC_SET_LENGTH, STG_SET_TIME, 0, 0)              ;
            
  while (cnt>0)
  { switch  (select)
    {  case 0  :
        // func statement   ;
        break   ;
       case 1   :
        // func statement   ;
        break   ;
       case 2    :
        valu = Read_Switch_Align()   ;
        break   ;
       case 3    :
        valu = Read_Switch_Home()    ;
        break   ;
       case 4    :
        nanoseconds= RSP78_init ()    ;
        break   ;
       case 5    :
        dutcyc = RSP78_Read_Sig_Period()        ;
        break   ;
       case 6    :
        while (mot>0)
        { pos=0   ;
          while (pos <18)
          { position=PWM_Positions[pos];
            SET_PWM9_DutyCycle(position)   ;
            delay(10000)  ;
            pos++       ;
          }
          --mot ;
        }
        break   ;
       case 7    :
        // statement ;
        break   ;
       case 8    :
        valu = A2D_c10_read()    ;
        break   ;
       case 9    :
        valu = A2D_c10_sample(5,1)  ;
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
//    PING I2C2 Decive Testing
// *********************************************      
        case 60    :
         stat=i2c2_ping(0x68)  ;      // Real Time Clock RTC
         break             ;
        case 61    :
         stat=i2c2_ping(0x39)   ;     // RGB Gesture
         break              ;
        case  62    :
         stat=i2c2_ping(0x76)   ;     //Temp Humidity Pressure
         break             ;
        case  63    :
         stat=i2c2_ping(0x1C)    ;    // Accelerometer & Magnetometer (Gyro separate address)
         break              ;
        case  64    :
         stat=i2c2_ping(0x6A)    ;    // Gyroscope (Accelerometer & Magnetometer separate address)
         break              ;
// *********************************************
//    RTC I2C2 Decive Testing
// *********************************************      
        case 70    :
//         i2c2_TalkToDevice(0x68, RTC_GET_LENGTH, PTR_SET_TIME, RTC_RSP_LENGTH, STG_GET_TIME)              ;  ;      // Real Time Clock Set Time
         delay(1)           ;
         break              ;
        case 71    :
         i2c2_ping(0x39)   ;     // RGB Gesture
         break              ;
        case  72    :
         i2c2_ping(0x76)   ;     //Temp Humidity Pressure
         break             ;
        case  73    :
         i2c2_ping(0x1C)    ;    // Accelerometer & Magnetometer (Gyro separate address)
         break              ;
        case  74    :
         i2c2_ping(0x6A)    ;    // Gyroscope (Accelerometer & Magnetometer separate address)
         break              ;
    }
// *********************************************
//     SWITCH ENDS
// ********************************************* 
    delay (1) ;
    cnt--       ;
  }     // end of while
  return (0);
  
}

 
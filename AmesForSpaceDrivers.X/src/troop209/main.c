/* 
 * File:   main.c
 * Author: Joe Pluta (?)
 *
 * Created BEFORE Nov 12, 2016, 3:10 PM
 * 
 * Revision Notes
 * Jan 17, 2017
 * 1. Added #include kernel.h to eliminate compiler warnings (kernel.h new file on Jan 17)
 * Jan 29 2017 merged by Harmon Bhasin
 */
// This version of the main application is configurable
// The program will attempt to read the SD Card to find
//   a config.ini file to drive the rest of the system

#include <nesi.h>
#include "SD_support.h"
#include "dateTime.h"
#include "../NormI2C/HdwrTest.h"

SDConfig config;

int main(void)
{

    nesi.init();
 //* comment out unless camera testing
//    usb.connect();

//   while (!button.isPressed());
//   while (button.isPressed());
//   usb.eject();
   
    
    SDConfigFile.get(&config);
//    if (config.kernelID == 0) config.kernelID = 16;

// Fix the kernelID to what is currently needed.     
// Not4: kernelID 19 is HdwrTest with diag file sizing, 18 is without file saving
//    config.kernelID = 16;  // Put a breakpoint here, and change kernelID to what's needed. 
    
    
//    sprintf(buf, "Kernel ID = %i\n\r", config.kernelID);
//    SDEventFile.writeln(buf);
   
    
    DateAndTime rtcStart = dateTime.parseStamp(config.rtc_start);
    DateAndTime expEnd = dateTime.parseStamp(config.exp_end);
    
    int rtcB4End = dateTime.cmp(rtcStart, expEnd);
    rtcB4End = dateTime.cmp(expEnd, expEnd);
    
    switch (config.kernelID) 
    {
        case 12: kernel12(); break;  // 0C - get/set external RTC
        case 13: kernel13(); break;  // 0D - USB connect to update config
        case 14: kernel14(); break;  // 0E - Write config
        case 15: kernel15(); break;  // 0F - Read config
        case 16: kernel16(); break;  // 10 - Camera test
        case 17: kernel17(); break;
        case 18: kernel18(); break;  // 12 - Happy Path
        case 19: kernel19(); break;  // 13 - Hardware Test
        default: kernel16(); break;
    }
     
}

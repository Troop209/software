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

static char logLabel[200];
static char buf[50];

int main(void)
{
    // initialize all modules
    nesi.init();
    system.init();

    int stat    = 0 ;   // capture status response from calls
    if(file1.open("launchDtTm.txt") != EOF){                                    //Open file datalog.txt and write SNS_Buffer
    
        String launchDT ; 
        //Harmon - uncomment after testing.
//        file1.read ((Byte *)launchDT,8);
//        launchDT = strcat(launchDT,"Read");
//        file1.write((Byte *)launchDT, 8);
// - Initialize I2C-RTC
//        stat=i2c2_TalkToDevice(0x68, 8,RTC_SET_TIME,  0, NullPtr)                    ; // set I2C RTC BCD string

        //rename the file 
        file1.rename("LaunchDTTMDeleted");
        file1.close()   ;
    }
    
    initSensors()       ; 
    setInternalClock();
    

    usb.connect();
    while (!button.isPressed());
    while (button.isPressed());
    usb.eject();
     
    //loadConfig();
    SDConfigFile.get(&config);
    if (config.kernelID == 0) config.kernelID = 16;

    //sprintf(buf, "Kernel ID = %i\n\r", config.kernelID);
    //SDEventFile.writeln(buf);
    
    switch (config.kernelID) {
        case 12: kernel12(); break;
        case 13: kernel13(); break;
        case 14: kernel14(); break;
        case 15: kernel15(); break;
        case 16: kernel16(); break;
        case 17: kernel17(); break;
        case 18: kernel18(); break;
        case 21: HdwrTest(); break;
    }
    
    return 0;
}

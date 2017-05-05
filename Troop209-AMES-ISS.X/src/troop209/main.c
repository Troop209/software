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
#include "../troop209hw/HdwrTest.h"

SDConfig config;

static char logLabel[200];
static char buf[50];

int main(void)
{
    nesi.init();
    system.init();

    // usb.connect();
    
    HdwrTest()  ;
    
   // NRM // while (!button.isPressed());
    // NRM // while (button.isPressed());
    // usb.eject();
     
    //loadConfig();
    SDConfigFile.get(&config);
    if (config.kernelID == 0) config.kernelID = 16;

//    sprintf(buf, "Kernel ID = %i\n\r", config.kernelID);
//    SDEventFile.writeln(buf);
/*   
    switch (config.kernelID) {
        case 12: kernel12(); break;
        case 13: kernel13(); break;
        case 14: kernel14(); break;
        case 15: kernel15(); break;
        case 16: kernel16(); break;
        case 17: kernel17(); break;
        case 18: kernel18(); break;
    }
 */    
}

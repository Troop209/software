#include <nesi.h>
#include <led.h>
#include "SD_support.h"

extern SDConfig config;

int kernel15(void)
{
    // initialize all modules
    ledR.init();
    
    //  Never ending loop
    //    1. Wait for pushbutton to be pressed
    //    2. Wait for it to be unpressed
    //    3. Log to the data logger
    
    int logNum = 0;
    char buf[30];
    
    while ( 1 ) {
        
        while (!button.isPressed());
        while (button.isPressed());

        sprintf(buf, "%s %i", config.label, logNum);
        SDEventFile.writeln(buf);
        ledR.dutycycle(100);
        delay(250);
        ledR.dutycycle(0);
        
        usb.connect();
        
        while (!button.isPressed());
        while (button.isPressed());

        usb.eject();
        
    }
    
    return 0;
}


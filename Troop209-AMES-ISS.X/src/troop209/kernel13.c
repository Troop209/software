#include <nesi.h>
#include <led.h>
#include "SD_support.h"

int kernel13(void)
{
    // initialize all modules
    ledR.init();
    ledB.init();
    
    //  Never ending loop
    //    1. Wait for pushbutton to be pressed
    //    2. Wait for it to be unpressed
    //    3. Log to the event logger
    
    int logNum = 0;
    char buf[20];
    
    while ( 1 ) {
        
        while (!button.isPressed());
        while (button.isPressed());

        sprintf(buf,"Log Number %i", logNum);
        SDEventFile.writeln(buf);
        ledR.dutycycle(100);
        ledB.dutycycle(100);
        delay(250);
        ledR.dutycycle(0);
        ledB.dutycycle(0);
        
    }
    
    return 0;
}


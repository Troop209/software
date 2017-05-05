//#include "../drivers/system.h"
//#include "../drivers/moisture.h"
//#include "../core/led.h"

#include <nesi.h>
#include <led.h>
#include <datalog.h>
//#include <file.h>
#include "../core/file.h"


int kernel18(void)
{
    // initialize all modules
    nesi.init();
    system.init();
    dataLog.init();
    ledR.init();
    
    usb.connect();
    while (!button.isPressed());
    while (button.isPressed());
    usb.eject();
        
    
    //  Never ending loop
    //    1. Wait for pushbutton to be pressed
    //    2. Wait for it to be unpressed
    //    3. Log to the data logger
    
    int logNum = 0;
    
    while ( 1 ) {
        
        while (!button.isPressed());
        while (button.isPressed());

        dataLog.add("Kernel18", ++logNum);
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


//#include "../drivers/system.h"
//#include "../drivers/moisture.h"
//#include "../core/led.h"

#include <nesi.h>
#include <led.h>
#include <datalog.h>
//#include <file.h>
#include "../core/file.h"

int kernel17(char lbl[20])
{
    nesi.init();
    
    // initialize all modules
    dataLog.init();
    ledR.init();

    //  Never ending loop
    //    1. Wait for pushbutton to be pressed
    //    2. Wait for it to be unpressed
    //    3. Log to the data logger
    
    int logNum = 0;
    
    while ( 1 ) {
        
        while (!button.isPressed());
        while (button.isPressed());

        dataLog.add(lbl, ++logNum);
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


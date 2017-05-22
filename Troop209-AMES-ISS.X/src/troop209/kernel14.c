#include <nesi.h>
#include <led.h>
#include "SD_support.h"

int kernel14(void)
{
    // initialize all modules
 
    //  Never ending loop
    //    1. Wait for pushbutton to be pressed
    //    2. Wait for it to be unpressed
    //    3. Log to the event logger
    //    4. Log to the data logger
    //    5. Connect the USB
    //    6. Wait for pushbutton to be pressed
    //    7. Wait for it to be unpressed
    //    8. Eject the USB
    
    int logNum = 0;
    char buf[20];
    
    sprintf(buf,"Log Number %i", logNum);
    SDEventFile.writeln(buf);
    SDDataFile.writeln("Data,123,Data,456");

    usb.connect();
    delay(100000);
    return 0;
}


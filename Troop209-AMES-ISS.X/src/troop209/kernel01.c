#include "../drivers/system.h"
#include "../drivers/led.h"
#include "../drivers/moisture.h"
#include "../drivers/powerDriver.h"
#include "../drivers/co2.h"
#include "../drivers/button.h"
#include "../drivers/usb.h"
#include "../drivers/sdcard.h"
#include "../drivers/dataLog.h"
#include "../drivers/camera.h"


int kernel01(void)
{
    // initialize all modules
    
    system.init();
    redLed.init();
    blueLed.init();
    sdcard.init();
    powerDriverA.init();
    powerDriverB.init();
    co2.init();
    moisture.init();
    sdcard.init();
    usb.init();
    button.init();
    dataLog.init();
    camera.on();

    int whatToDo = 1;
    int written = 0;
    
    /*  Simple test program to make sure
     *  environment is set up properly
     *  and code can be compiled
    */

    //dataLog.add("My Test", 0);
    //dataLog.add("My Test", 1);
    
    while(1)
    {
        switch (whatToDo) {
            case 1:
                CopCar();
                break;
            case 2:
                if (written == 0) {
                    dataLog.add("Joe Says", 22);
                    written = 1;
                }
                break;
            default:
                flash(whatToDo);
        }
        
        if (button.isPressed()) {
            while (button.isPressed()){}
            whatToDo = (whatToDo % 3) + 1;
        }
    }
    return 0;
}

void CopCar() {
    blueLed.dutycycle(0);
    redLed.dutycycle(100);
    delay(250);
    blueLed.dutycycle(100);
    redLed.dutycycle(0);   
    delay(250);
}

void flash(int flashes) {
    blueLed.dutycycle(0);
    redLed.dutycycle(100);
    delay(250);
    redLed.dutycycle(0);
    while (flashes--) {
        delay(250);
        blueLed.dutycycle(100);
        delay(250);
        blueLed.dutycycle(0);
    };
}

void showError(int code) {
    while(1) flash(code);
}
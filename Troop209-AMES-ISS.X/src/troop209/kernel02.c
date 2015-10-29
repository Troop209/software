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


int kernel02(void)
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
    //camera.on();
    
    if (xcamera.init()) showCode(2); 
    
    showCode(3);
    
    return (0);
}

void showCode(int code) {
    while(1) flash(code);
}
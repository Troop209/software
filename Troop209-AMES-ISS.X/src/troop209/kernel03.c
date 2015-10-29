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


int kernel03(void) 
{
    // initialize all modules
    
    system.init();
    redLed.init();
    blueLed.init();
    sdcard.init();
    moisture.init();
    dataLog.init(); 

    dataLog.add("Start: ",0);
    dataLog.add("Q1: ",moisture.readQ1());
    dataLog.add("Q2: ",moisture.readQ2());
    dataLog.add("Q3: ",moisture.readQ3());
    dataLog.add("Q4: ",moisture.readQ4());
    dataLog.add("End: ",0);

    showCode(3);
    
    return (0);
}

//void showCode(int code) {
//    while(1) flash(code);
//}
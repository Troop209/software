#include "../drivers/system.h"
#include "../drivers/led.h"


int kernel05(void) 
{
    // initialize all modules
    
    system.init();
    redLed.init();
    blueLed.init();

    blueLed.dutycycle(100);
    redLed.dutycycle(100);
    delay(2000);

    while(1)
    {
        blueLed.dutycycle(0);
        redLed.dutycycle(100);   
        delay(200);
        blueLed.dutycycle(25);
        redLed.dutycycle(75);
        delay(200);
        blueLed.dutycycle(50);
        redLed.dutycycle(50);
        delay(200);
        blueLed.dutycycle(75);
        redLed.dutycycle(25);
        delay(200);
        blueLed.dutycycle(100);
        redLed.dutycycle(0);
        delay(200);
    }
    
    return(1);
            
}

//void showCode(int code) {
//    while(1) flash(code);
//}
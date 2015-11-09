#include "../drivers/system.h"
#include "pwm.h"


int kernel06(void)  
{
    // initialize all modules
    
    system.init();
    pwm1.init(0b111);
    pwm2.init(0b111);
    
    unsigned duty = 0;
    
    while(1)
    {
        for (duty = 0; duty <= 100; duty++)
        {
            pwm1.turnOn( 6400, duty);
            pwm2.turnOn( 6400, 100-duty);
            delay(20);
        }
        for (duty = 0; duty <= 100; duty++)
        {
            pwm1.turnOn( 6400, 100-duty);
            pwm2.turnOn( 6400, duty);
            delay(20);
        }
    }
    
    return (0);
}

//void showCode(int code) {
//    while(1) flash(code);
//}
#include "../drivers/system.h"
#include "tmr1.h"
#include "pwm.h"


int kernel07(void)  
{
    // initialize all modules
    
    system.init();
    pwm1.init(4);
    
    timer1.start( 3, 0);  // Set to 1ms clock
    pwm1.turnOn( 60000, 50);
    delay(10000);
    
    pwm1.turnOff();
    delay(5000);
    
    return (0);
}

//void showCode(int code) {
//    while(1) flash(code);
//}
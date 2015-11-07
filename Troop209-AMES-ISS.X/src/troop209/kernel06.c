#include "../drivers/system.h"
#include "pwm.h"


int kernel06(void)  
{
    // initialize all modules
    
    system.init();
    pwm1.init();
    
    pwm1.turnOn( 6400, 50);
    showCode(3);
    
    return (0);
}

//void showCode(int code) {
//    while(1) flash(code);
//}
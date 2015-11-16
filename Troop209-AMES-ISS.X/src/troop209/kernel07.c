#include "../drivers/system.h"
#include "tmr1.h"
#include "pwm.h"


int kernel07(void)  
{
    // initialize all modules
    
    system.init();
    pwm1.init(PWM_TIMER_SELECTION_TIMER1);
     
    timer1.start( TIMER_PRE_SCALE_64, 0);  // Set to 1ms clock
    pwm1.turnOn( 60000, 50);
    delay(10000);
    
    pwm1.turnOff();
    delay(5000);
    
    return (0);
}

//void showCode(int code) {
//    while(1) flash(code);
//}
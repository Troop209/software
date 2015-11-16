#include "../drivers/system.h"
#include "tmr1.h"
#include "pwm1.h"


int servomortor(void)  
{
    // initialize all modules
    
    system.init();
    
    // Per Norm's email on 11/14
    
    // -90 degrees
    pwm1.initTimer1(10000,250);
    timer1.start( TIMER_PRE_SCALE_64, 0);  
    pwm1.turnOnPwm1();    
    delay(10000);    
    pwm1.turnOff();

    delay(10000);
    
    // 0 degrees
    pwm1.initTimer1(10000, 375);
    pwm1.turnOnPwm1();
    delay(10000);    
    
    
    // TODO:  Add turn 90 degrees with value
    // pwm1.initTimer1(10000, 500);
    // pwm1.turnOnPwm1();
    // delay(10000);    
    return (0);
}

//void showCode(int code) {
//    while(1) flash(code);
//}
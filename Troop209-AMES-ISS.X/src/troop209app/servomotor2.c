#include "../core/system.h"
#include "tmr5.h"
#include "pwm9.h"


int servomotor2(void)  
{
   // initialize all modules
    
    system.init();
    ServoInit();
    ServoRun();
    
    return (0);
}

void ServoInit() {
    pwma9.init(PWM_TIMER_SELECTION_TIMER5);  // degree calculation is -2.5 * degree + 250
    timer5.start(2,0);
}

void ServoRun()
{
//    goTo( -90, 500);
//    delay(2000);
//    goTo( -60, 500);
//    delay(2000);
//    goTo( -30, 500);
//    delay(2000);
//    goTo( 0, 500);
//    delay(2000);
    goTo( 30, 500);
    delay(2000);
    goTo( 60, 500);
    delay(2000);
    goTo( 90, 500);
    delay(2000);
    goTo( 60, 500);
    delay(2000);
    goTo( 30, 500);
    delay(2000);
//    goTo( 0, 500);
//    delay(2000);
//    goTo( -30, 500);
//    delay(2000);
//    goTo( -60, 500);
//    delay(2000);
//    goTo( -90, 500);
//    delay(2000);
    
    delay(1000);
}

void goTo(int degrees, int micros)
{
    int width = 350 - (degrees * 2.5);
    pwma9.turnOn( 10000, width);
    delay(micros);
}
#include "../core/system.h"
#include "tmr1.h"
#include "pwm1.h"


int servomotor2(void)  
{
   // initialize all modules
    
    system.init();
    ServoInit();
    ServoRun();
    
    return (0);
}

void ServoInit() {
    pwma1.init(PWM_TIMER_SELECTION_TIMER5);  // degree calculation is -2.5 * degree + 250
    timer5.start(2,0);
}

void ServoRun()
{
    goTo( -90, 500);
    goTo( -60, 500);
    goTo( -30, 500);
    goTo( 0, 500);
    goTo( 30, 500);
    goTo( 60, 500);
    goTo( 90, 500);
    goTo( 60, 500);
    goTo( 30, 500);
    goTo( 0, 500);
    goTo( -30, 500);
    goTo( -60, 500);
    goTo( -90, 500);
    
    delay(1000);
}

void goTo(int degrees, int micros)
{
    int width = 350 - (degrees * 2.5);
    pwma1.turnOn( 10000, width);
    delay(micros);
}
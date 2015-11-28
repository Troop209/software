#include "../drivers/system.h"
#include "tmr1.h"
#include "pwm1.h"


int servomortor(void)  
{
   // initialize all modules
    
    system.init();
    pwm1.init(4);  //
    
    timer1.start( 2, 0);  // Set to 1ms clock and using 24 prescaler not 64
//    pwm1.turnOn( 60000, 50);
    pwm1.turnOn( 10000, 250);
    delay(1000);
//    pwm1.turnOff();
//    delay(1000);

    pwm1.turnOn( 10000, 375);
    delay(1000);
 //   pwm1.turnOff();
 //   delay(1000);
    
    pwm1.turnOn( 10000, 500);
    delay(10000);

//    pwm1.turnOff();
    
//    delay(5000);
    return (0);}

//void showCode(int code) {
//    while(1) flash(code);
//}
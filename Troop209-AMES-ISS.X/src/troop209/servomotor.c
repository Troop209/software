#include "../core/system.h"
#include "tmr1.h"
#include "pwm1.h"


int servomotor(void)  
{
   // initialize all modules
    
    system.init();
    pwma1.init(4);  // degree calculation is -2.5 * degree + 250
    timer1.start(2,0);
    
    pwma1.turnOn( 10000, 575); // -90
    delay(1000);

    pwma1.turnOn( 10000, 500);  // -60
    delay(1000);
    
    pwma1.turnOn( 10000, 425);  // -30
    delay(10000);

    pwma1.turnOn( 10000, 350);  // 0
    delay(10000);

    pwma1.turnOn( 10000, 275);  // 30
    delay(10000);

    pwma1.turnOn( 10000, 200);  // 60
    delay(10000);

    pwma1.turnOn( 10000, 125); // 90
    delay(10000);

    return (0);
}

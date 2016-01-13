#include "../drivers/system.h"
#include "../drivers/moisture.h"
#include "../drivers/led.h"
#include "tmr1.h"
#include "pwm.h"


int kernel09(void)  
{
    int current, previous; 
    
    // initialize all modules
    
    system.init();
    moisture.init();
    previous = -1;
    
    while (1) {
        
        current = getDips();
        
        switch (current) {
            
            case 0:
                if (previous != 0)
                    CopCarInit();
                CopCar();
                break;
                
            case 1:
                if (previous != 1)
                    PWMInit();
                PWMRun();
                break;
                
            case 2:
                if (previous != 2)
                    ServoInit();
                ServoRun();
                break;
                
            default:
                if (previous != current)
                    CopCarInit();
                flash(current);
        }
        
        previous = current;
        
    }

    return (0);
}

int getDips() {
    int q1, dips;
    
      q1 = moisture.readQ1();
      
      if (q1 > 345)
          dips = 0b000;
      else if (q1 > 310)
          dips = 0b001;
      else if (q1 > 270)
          dips = 0b010;
      else if (q1 > 216)
          dips = 0b011;
      else if (q1 > 156)
          dips = 0b100;
      else if (q1 > 99)
          dips = 0b101;
      else if (q1 > 33)
          dips = 0b110;
      else 
          dips = 0b111;
      
      return dips;
}

void PWMInit() {
    pwm1.init(0b100);
    timer1.start( 3, 0);  // Set to 1ms clock
}
    
void PWMRun() {
    unsigned q3;
    q3 = moisture.readQ3();
    pwm1.turnOn( q3*60, 5);
    delay(250);
}

void CopCarInit() {
    redLed.init();
    blueLed.init();
}

void CopCar() {
    blueLed.dutycycle(0);
    redLed.dutycycle(100);
    delay(250);
    blueLed.dutycycle(100);
    redLed.dutycycle(0);   
    delay(250);
}

void flash(int flashes) {
    blueLed.dutycycle(0);
    redLed.dutycycle(100);
    delay(250);
    redLed.dutycycle(0);
    while (flashes--) {
        delay(250);
        blueLed.dutycycle(100);
        delay(250);
        blueLed.dutycycle(0);
    };
}


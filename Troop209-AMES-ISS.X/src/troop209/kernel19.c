
#include <nesi.h>
#include "pwm_motor.h"
#include <string.h>;

int kernel19(void)
{
    // initialize all modules
    motor.init(); 
    
    //  Never ending loop
    //    1. Wait for pushbutton to be pressed
    //    2. Wait for it to be unpressed
    //    3. Log to the data logger
    
    int logNum = 0;
    int valu = 0      ;   //returned value
    char buf[20];
    
    while ( 1 ) {
        
        sprintf(buf, "Kernel20 - %i", ++logNum);
        SDEventFile.writeln(buf);
        
        _TRISE6 = 0;               // configure port as output
        _RE6 = 1; // set the output (active high)  Turn on 6Volt  supply 
          
        valu = motor.turn(2048)        ;//180
        delay(5000)    ;
        
        valu= motor.turn(0)        ;//0
        delay(5000)    ;
        
        valu = motor.turn(512)        ;//45
        delay(5000)    ;
        
        valu = motor.turn(1024)        ;//90
        delay(5000)    ;
        
        valu = motor.turn(1536)        ;//135
        delay(5000)    ;
        
        valu = motor.turn(2560)        ;//225
        delay(5000)    ;
        
        valu = motor.turn(3072)        ;//270
        delay(5000)    ;
        
        valu = motor.turn(3584)        ;//315
        delay(5000)    ;
        
        usb.connect();
        while (!button.isPressed());
        while (button.isPressed());
        usb.eject();
        
    }
    
    return 0;
}



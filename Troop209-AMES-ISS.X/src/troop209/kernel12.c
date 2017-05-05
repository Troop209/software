#include <nesi.h>
#include <led.h>

int kernel12(void)
{
    // initialize all modules
    ledR.init();
    ledB.init();
    
    while ( 1 ) {
        
        while (!button.isPressed());
        while (button.isPressed());

        ledR.dutycycle(100);
        ledB.dutycycle(100);

        while (!button.isPressed());
        while (button.isPressed());

        ledR.dutycycle(0);
        ledB.dutycycle(0);
        
    }
    
    return 0;
}


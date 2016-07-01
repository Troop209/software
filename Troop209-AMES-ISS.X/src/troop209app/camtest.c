#include <nesi.h>
#include <led.h>
#include <camera.h>
#include <button.h>

#define STEP_WAIT 3000

int camtest(void)
{
    // initialize all modules
    nesi.init();
    system.init();
    camera.init();
    CopCarInit();
    
    int mirrorPicNumber = 800;
    
    //  Never ending loop
    //    1. Wait for pushbutton to be pressed
    //    2. Wait for it to be unpressed
    //    3. Flash LED 1
    //    4. Take picture camera 1
    //    5. Flash LED 2
    
    while ( 1 ) {
        
        while (!button.isPressed());

        while (button.isPressed());

        ledB.dutycycle(0);
        ledR.dutycycle(100);
        delay(250);
        ledR.dutycycle(0);
    
        char filename[32] = {0};
        sprintf(filename, "mirrorpic%i.jpg", ++mirrorPicNumber);
        camera.getPix(filename);
        
        ledR.dutycycle(0);
        ledB.dutycycle(100);
        delay(250);
        ledB.dutycycle(0);
    
    }
    
    return 0;
}



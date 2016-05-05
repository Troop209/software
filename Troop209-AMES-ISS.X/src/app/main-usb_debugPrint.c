#include <nesi.h>

#include "motor.h"

int main(void) {
        system.init();

        usb.connect();

    char input[64] = {0};
    int bytesRead = 0;
    char message[128] = {0};
    
    while(1)
    {
        sprintf(message, "Time is: %s\r\n", dateTime.getStamp());
        usb.print(message);
        wait(1500);

        
        if((bytesRead = usb.read(input,64)))
        {
            input[bytesRead] = '\0'; // terminate string
            usb.printf("At time %s, %d bytes of data received:\r\n\"%s\"\r\n\r\n",
                    dateTime.getStamp(), bytesRead, input);
        }
    }
    
}
int mainPwm(void) {
    int PWM_Positions[18] = {16000, 17000, 18000, 19000, 20000, 21000, 22000, 23000, 24000,
        25000, 26000, 27000, 28000, 29000, 30000, 31000, 32000, 16000};

    // Initialization begins here

    system.init();
    motor.init();

    int mot = 5000;
    int pos = 0;
    int position = 0;

    while (mot > 0) {
        pos = 0;
        while (pos < 18) {
            position = PWM_Positions[pos];
            motor.dutycycle(position);
            delay(10000);
            pos++;
        }
        --mot;
    }
    
    return (0);
}

int mainCamera(void) {
    nesi.init();

    // Set military time and Date
    DateAndTime timeTemp;

    // time = 12:52:50
    timeTemp.hour = 16;
    timeTemp.minute = 0;
    timeTemp.second = 0;

    // date = 10/29/13
    timeTemp.month = NOVEMBER;
    timeTemp.day = 25;
    timeTemp.year = 13;

    dateTime.set(timeTemp);

    //   camera.on();
    char filename[32] = {0};
    sprintf(filename, "1_%s.jpg", dateTime.getStamp());

    // the ':' character is an invalid character, so it needs to be changed
    String temp = filename;
    while (*temp) {
        if (*temp == ':')
            *temp = '.';
        ++temp;
    }
    
    // first character is 1 for file name
    Boolean b = camera2.getPix(filename);
    
    // change first character to 2 for file name to denote 2nd camera
    filename[0] = '2';
    b = camera2.getPix(filename);
    //  camera.off();

    return (0);
}

int main2(void) {
    // initialize all modules
    nesi.init();

    usb.connect();

    char input[64] = {0};
    int bytesRead = 0;
    Boolean debugEnable = YES;
    usb.setDebug(ON);

    while (1) {
        if ((bytesRead = usb.read(input, 64))) {
            input[bytesRead] = '\0'; // terminate string
            usb.debugPrint("At time %s, %d bytes of data received:\r\n\"%s\"\r\n\r\n",
                    dateTime.getStamp(), bytesRead, input);

            if (debugEnable == YES) {
                debugEnable = NO;
                usb.setDebug(OFF);
                while (!usb.print("Disabling Debug Print. The next input will not be printed.\r\n")); // keep trying until successful
            } else {
                debugEnable = YES;
                usb.setDebug(ON);
                usb.print("Enabling Debug Print.\r\n"); // don't retry print if unsuccessful
            }
        }
    }

    return 0;
}

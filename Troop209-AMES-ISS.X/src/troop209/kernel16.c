#include <nesi.h>
#include <string.h>
#include "SD_support.h"

// stores timestamped picture
Boolean takePicture(void)
{
    char filename[40] = {0};
    
    // the ':' character is an invalid character, so it needs to be changed
    sprintf(filename, "%s-%s-cam1.jpg", "picture", dateTime.getStamp());
    fixcolons(filename);
    camera.getPix(filename);

    delay(1000);

    sprintf(filename, "%s-%s-cam2.jpg", "picture", dateTime.getStamp());
    fixcolons(filename);
    camera2.getPix(filename);

    return (0);
}

void fixcolons(char *temp)
{
    while(*temp)
    {
        if(*temp == ':')
            *temp = '.';
        
        ++temp;
    }
}

int kernel16(void)
{
    // Initializes NESI modules
    nesi.init();

    // Set military time and Date
    DateAndTime timeTemp;
    // time = 13:00:00
    timeTemp.hour   = 13;
    timeTemp.minute = 0;
    timeTemp.second = 0;

    // date = 05/28/17
    timeTemp.month = 05;
    timeTemp.day   = 28;
    timeTemp.year  = 17;

    dateTime.set(timeTemp);

    usb.eject();    
    dataLog.add("Start Camera Test", 0);

    wait(1000);
    int pictureloop=0;
    while(pictureloop < 5)
    {
            pictureloop++;
            takePicture(); // 
            delay(1000);
     }
    usb.connect();
    while(1){} //infinite loop
}

#include <nesi.h>
#include <camera.h>

#include "non_pwm_led.h"

#define STEP_WAIT 3000

int camera_main(void)
{
    nesi.init();

    // Set military time and Date
    DateAndTime timeTemp;

    // -------------------------------------------------------------------------------
    // MA_TODO:  Update time to our meeting time
    // -------------------------------------------------------------------------------
    timeTemp.hour = 16;
    timeTemp.minute = 0;
    timeTemp.second = 0;

    // -------------------------------------------------------------------------------
    // MA_TODO:  Update to demo date (hint:  July 7, hint2:  make sure you have right year)
    // -------------------------------------------------------------------------------
    timeTemp.month = NOVEMBER;
    timeTemp.day = 25;
    timeTemp.year = 13;

    dateTime.set(timeTemp);

    char filename[32] = {0};
    // -------------------------------------------------------------------------------
    // MA_TODO:  Update this to filename that makes sense that includes the following
    // 1.  date time (YYMMDDD)
    // 2.  camera #
    // 3.  servo position (hard code some value for now)
    // -------------------------------------------------------------------------------
    char camera_num = "1";
    char servo_pos = "2";
    sprintf(filename, "%s_%s_%s.jpg", camera_num, servo_pos, dateTime.getStamp());

    // the ':' character is an invalid character, so it needs to be changed
    String temp = filename;
    while (*temp) {
        if (*temp == ':')
            *temp = '.';
        ++temp;
    }

    // -------------------------------------------------------------------------------
    // MA_TODO:  If LED1 turns on the other side, then we will need to change the code to led2 from led1
    //           Make sure to change line 64 and 66 to be led1
    // -------------------------------------------------------------------------------
    led1.on();
    Boolean b = camera.getPix(filename);
    led1.off();
    
    camera_num = '2';
    // -------------------------------------------------------------------------------
    // MA_TODO:  If camera pos in the filename you selected above is NOT first character, 
    //           change 0 in filename[0] to correct position
    // -------------------------------------------------------------------------------
    filename[0] = camera_num;
    led2.on();
    b = camera2.getPix(filename);
    led2.off();

    return (0);
}



#include <nesi.h>
#include <camera.h>

#include "../norm/Switch_Align_Home.h"

#define STEP_WAIT 3000

int camera_main(void)
{
    nesi.init();

    // Set military time and Date
    DateAndTime timeTemp;

    // -------------------------------------------------------------------------------
    // MA_TODO:  Update time to our meeting time
    // -------------------------------------------------------------------------------
    timeTemp.hour = 18;
    timeTemp.minute = 0;
    timeTemp.second = 0;

    // -------------------------------------------------------------------------------
    // MA_TODO:  Update to demo date (hint:  July 7, hint2:  make sure you have right year)
    // -------------------------------------------------------------------------------
    timeTemp.month = JULY;
    timeTemp.day =14;
    timeTemp.year = 16;

    dateTime.set(timeTemp);

    char filename[32] = {0};
    // -------------------------------------------------------------------------------
    // MA_TODO:  If LED1 turns on the other side, then we will need to change the code to led2 from led1
    //           Make sure to change line 64 and 66 to be led1
    // -------------------------------------------------------------------------------
    char camera_num = '1';
    char servo_pos = '2';
    int cnt = 0;
    while(1) {
        camera_num = '2';
        cnt++;
        sprintf(filename, "%c_%c_%u_%s_c.jpg",camera_num, servo_pos, cnt, dateTime.getStamp());
        String temp = filename;
        while (*temp) {
            if (*temp == ':')           
                *temp = '.';
            ++temp;
        }
        
        setOnLED2();
        delay(5000);
        Boolean b = camera2.getPix(filename);
        setOffLED2();

        delay(250);
        camera_num = '1';
        // -------------------------------------------------------------------------------
        // MA_TODO:  If camera pos in the filename you selected above is NOT first character, 
        //           change 0 in filename[0] to correct position
        // -------------------------------------------------------------------------------
        filename[0] = camera_num;
        setOnLED1();
        delay(5000);
        b = camera.getPix(filename);
        setOffLED1();

        delay(5000);

        setOnLED1();
        setOnLED2();

        delay(5000);
        setOffLED1();
        setOffLED2();
        delay(5000);
    }
   return (0);
}



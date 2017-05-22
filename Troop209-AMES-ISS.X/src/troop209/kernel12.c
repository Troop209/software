#include <nesi.h>

/**
 * One time setup of xRTC
 * @return 
 */
int kernel12(void)
{
    nesi.init();
    i2c2_init(75); // Initialize I2C2 and set Baud Rate
    i2c2_reset_bus(); // Reset I2C Bus/Network

    DateAndTime timeTemp;

    // time = 7:01:00
    timeTemp.hour   = 6;
    timeTemp.minute = 58;
    timeTemp.second = 0;

    // date = 10/29/13
    timeTemp.month = MAY;
    timeTemp.day   = 21;
    timeTemp.year  = 17;

    dateTime.set(timeTemp);
    String xRTCTime1 = dateTime.toStamp(timeTemp);
    char xRTCTime[18]={"05/21/17 06:35:00\n"};
    int stat = getI2C2_RTCTime(xRTCTime);
    stat = setI2C2_RTCTime(xRTCTime1);    
    if (stat == 0) {
        wait(1000); // wait 1 sec
        stat = getI2C2_RTCTime(xRTCTime);
    }
    return 0;
}


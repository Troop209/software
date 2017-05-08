#include "experiment_initialize.h"
#include "SD_support.h"
#include "dateTime.h"
#include "system.h"
#include "usb.h"
#include "sdcard.h"
#include "dataLog.h"
#include "camera.h"
#include "dateTime.h"
#include "file.h"
#include "../troop209hw/I2CDrivers.h"
#include "../troop209hw/Analog2Digital.h"

SDConfig config;

static void setInternalRTC(char *xRTC);
static void initializeHardware();

void initialize_experiment(void) {
    extern signed long SNS_Temperature;
    
    initializeHardware();

    // read configuration file
    SDConfigFile.get(&config);

    // Delay is in ms and so need to convert time in config 
    int defrostWaitDuration = config.defrost_wait_duration_min * 60 * 1000;

    // read external rtc
    int stat = 0; // capture status response from calls
    char xRTC[18] = {"00/00/00 00:00:00\n"};
    stat = getI2C2_RTCTime(xRTC);
    setInternalRTC(xRTC);

    while (SNS_Temperature < 50) {
        readTHP();
        delay(defrostWaitDuration);
    }
}

void setExternalRTC() {
        String launchDT ; 
        //Harmon - uncomment after testing.
        file1.read ((Byte *)launchDT,8);
        launchDT = strcat(launchDT,"Read");
        file1.write((Byte *)launchDT, 8);
// - Initialize I2C-RTC
// (TODO-Figure out where RTC_SET_TIME)
//        short stat=i2c2_TalkToDevice(0x68, 8,RTC_SET_TIME,  0, NullPtr)                    ; // set I2C RTC BCD string

        //rename the file 
        file1.rename("LaunchDTTMDeleted");
        file1.close()   ;
}

/**
 * Converts externalRTC to internalRTC
 */
static void setInternalRTC(char *xRTC) { // Returns point if stat == 0 else returns null ptr
    //char   RTC_I2C_TimeStamp[18]    ;
    // update internal rtc
    DateAndTime timeTemp;

    static const int month_pos = 1;
    static const int day_pos = 4;
    static const int year_pos = 7;
    static const int hr_pos = 9;
    static const int min_pos = 12;

    // time = 12:52:50
    char value[2]; //MM1,2 DD4,5 YY7,8 HH 9, 10 MM 12, 13 SS 15, 16

    value[0] = xRTC[hr_pos];
    value[1] = xRTC[hr_pos + 1];
    timeTemp.hour = atoi(value);

    value[0] = xRTC[min_pos];
    value[1] = xRTC[min_pos + 1];
    timeTemp.minute = atoi(value);
    timeTemp.second = 0;

    value[0] = xRTC[month_pos];
    value[1] = xRTC[month_pos + 1];
    timeTemp.month = atoi(value);

    value[0] = xRTC[day_pos];
    value[1] = xRTC[day_pos + 1];
    timeTemp.day = atoi(value);

    value[0] = xRTC[year_pos];
    value[1] = xRTC[year_pos + 1];
    timeTemp.year = atoi(value);

    dateTime.set(timeTemp);
}

/**
 * Initialize only hardware used by project
 */
static void initializeHardware() {
    system.init();
    
    sdcard.init();
    usb.init();
    dataLog.init();
    camera.init();
    camera2.init();
    dateTime.init();

    i2c2_init(75); // Initialize I2C2 and set Baud Rate
    i2c2_reset_bus(); // Reset I2C Bus/Network

    initA2D(); // Configure Q1-Q4 Analog Inputs. Q3 Currently used for VMot check.

    int stat = initSensors(); // set up sensors // returns Error count of # sensors with errors. 0 Desired

    stat = i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr); // clear I2C2 diagnostic counters   

}
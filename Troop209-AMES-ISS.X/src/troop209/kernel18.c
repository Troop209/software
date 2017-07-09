/* ----------------------------------------------------
 * This is the single file version of the happy path 
 * ----------------------------------------------------
 */
#include "non_pwm_led.h"
#include <nesi.h>
#include <datalog.h>
#include "../core/file.h"
#include "../NormI2C/I2CDrivers.h"
#include "SD_support.h"
#include "../NormI2C/RadiationSensorDriver.h"
#include "../NormI2C/multiMotorDrivers.h"
#include "../NormI2C/Analog2Digital.h"
#include "../NormI2C/Encoder.h"
#include "../NormI2C/SensorDrivers.h"

void replaceColonWithPeriod(char *temp) {
    while (*temp) {
        if (*temp == ':')
            *temp = '.';

        ++temp;
    }
}

/**
 * Read sensor, format sensor and write the sensor to SDDataFile
 */
void handleSensor() {
    extern signed long SNS_Buffer[128];

    readSensors();
    SDDataFile.writeln(SNS_Buffer);
}
static const int enableMotor = 3;
static const int speed = 7;
static const int motPosition[9] = {0, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096};

void run_experiment(void) {

    handleSensor();

    int mot_inc;
    char filename[40] = {0};
    int stat;


    for (mot_inc = 0; mot_inc < 8; mot_inc++) {
        // move motor
        stat = checkCarousel(enableMotor, motPosition[mot_inc], speed);
        if (stat == 0) // functionSUCCESS
        {
            stat = moveCarousel(enableMotor, motPosition[mot_inc], speed);
        }

        // Camera1 sequence
        led1.on();
        sprintf(filename, "%s-%s-cam1.jpg", config.label, dateTime.getStamp());
        replaceColonWithPeriod(filename);
        camera.getPix(filename);
        led1.off();

        // Camera 2 sequence
        led2.on();
        sprintf(filename, "%s-%s-cam2.jpg", config.label, dateTime.getStamp());
        replaceColonWithPeriod(filename);
        camera2.getPix(filename);
        led2.off();
    }

    handleSensor();
}

void init_experiment() {
    nesi.init();
    SDConfigFile.get(&config);
    
    i2c2_init(75); // Initialize I2C2 and set Baud Rate
    i2c2_reset_bus(); // Reset I2C Bus/Network

    initA2D(); // Configure Q1-Q4 Analog Inputs. Q3 Currently used for VMot check.

    int stat = initSensors(); // set up sensors // returns Error count of # sensors with errors. 0 Desired

    stat = i2c2_TalkToDevice(I2C_CLR_DIAG, 0, NullPtr, 0, NullPtr); // clear I2C2 diagnostic counters   
}

void shutdown_experiment() {
    usb.connect();
    delay(10000);
}

/**
 * Only needed to get the dateTime without RTC
 * @return 
 */
int fakeoutTime() {
        DateAndTime timeTemp;

    // time = 7:01:00
    timeTemp.hour   = 6;
    timeTemp.minute = 58;
    timeTemp.second = 0;

    // date = 10/29/13
    timeTemp.month = MAY;
    timeTemp.day   = 27;
    timeTemp.year  = 17;

    dateTime.set(timeTemp);
    return 0;
}

int kernel18(void) {
    init_experiment();
    fakeoutTime();
    DateAndTime expEnd = dateTime.parseStamp(config.exp_end);
    DateAndTime expWait = dateTime.parseStamp(config.exp_wait_duration);
    Sint dtComp = dateTime.cmp(expEnd,dateTime.get());
    while (dtComp < 0) {
        // -- begin run_experiment
        handleSensor();

        int mot_inc;
        char filename[40] = {0};
        int stat;


        for (mot_inc = 0; mot_inc < 8; mot_inc++) {
            // move motor
            stat = checkCarousel(enableMotor, motPosition[mot_inc], speed);
            if (stat == 0) // functionSUCCESS
            {
                stat = moveCarousel(enableMotor, motPosition[mot_inc], speed);
            }

            // Camera1 sequence
            led1.on();
            sprintf(filename, "%s-%s-cam1.jpg", config.label, dateTime.getStamp());
            replaceColonWithPeriod(filename);
            camera.getPix(filename);
            led1.off();

            // Camera 2 sequence
            led2.on();
            sprintf(filename, "%s-%s-cam2.jpg", config.label, dateTime.getStamp());
            replaceColonWithPeriod(filename);
            camera2.getPix(filename);
            led2.off();
        }

        handleSensor();
        
        // -- end run_experiment
        // run_experiment();

        // each loop wait duration
        DateAndTime expResult = dateTime.add(dateTime.get(), expWait);
        Sint rotationComp = dateTime.cmp(expResult, dateTime.get());
        while ( rotationComp < 0) {
            int position = 0;
            int stat;
            
            readRadiation();
            readEncoder();
            stat = checkCarousel(enableMotor, motPosition[position], speed);
            if (stat == 0) // functionSUCCESS
            {
                stat = moveCarousel(enableMotor, motPosition[position], speed);
            }
            if (position < 9) {
                position++;
            } else {
                position = 0;
            }
            rotationComp = dateTime.cmp(expResult, dateTime.get());
        }
        dtComp = dateTime.cmp(expEnd,dateTime.get());
    }
    
    // -- begin shutdown 
    usb.connect();
    delay(10000);
    // -- end shutdown
    // shutdown_experiment();

    return 0;
}



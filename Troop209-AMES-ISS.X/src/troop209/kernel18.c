/* ----------------------------------------------------
 * This is the single file version of the happy path 
 * ----------------------------------------------------
 */
#include "non_pwm_led.h"
#include <nesi.h>
#include <datalog.h>
#include "../core/file.h"
#include "../normi2c/I2CDrivers.h"
#include "../normi2c/multiMotorDrivers.h"
#include "SD_support.h"

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
//    formatSensors(SNS_Buffer);
    
    SDDataFile.writeln(SNS_Buffer);
}
static const int enableMotor = 3;  //
static const int speed = 7;
static const int motPosition[9] = {0, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096};

void run_experiment(void) {

    handleSensor();

//    int mot_inc;
    char filename[40] = {0};

// Moved the Motor code  to the main loop to support background tasks(JRM)
//    int stat;
//    for (mot_inc = 0; mot_inc < 8; mot_inc++) {
        // move motor
//        stat = checkCarousel(enableMotor, motPosition[mot_inc], speed);
//        if (stat == 0) // functionSUCCESS
//        {
//            stat = moveCarousel(enableMotor, motPosition[mot_inc], speed);
//        }

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

//    handleSensor();
//}

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

int kernel18(void) {
    init_experiment();
    DateAndTime expEnd = dateTime.parseStamp(config.exp_end);
    DateAndTime expWait = dateTime.parseStamp(config.exp_wait_duration);

//    
// Run Happy path until we reach expEnd, the closing date of the experiment
//
    while (dateTime.cmp(dateTime.get(), expEnd) < 0) 
    {
//      Calculate time for next round of experiments
        DateAndTime expResult = dateTime.add(dateTime.get(), expWait);
        extern int motLookUp ;  //Start motor at position 0
        motLookUp = 0 ;
        extern int motComplete ; // reset motor rotation complete flag
        motComplete = 0 ;
        //
//Stay in this loop until time to initiate next round of experiments
        while (dateTime.cmp(dateTime.get(), expResult) > 0) 
        {
            int stat;
//
// Background tasks, keep running through these          
// Run readRadiation, readEncoder, and Motor calls in background loop
//
            readRadiation();
            readEncoder();
            stat = checkCarousel(enableMotor, motPosition[motLookUp], speed);
            if (stat == 0) // functionSUCCESS
            {
                stat = moveCarousel(enableMotor, motPosition[motLookUp], speed);
            }
//
// JRM Check moveStatus == COMPLETE
// When Motor is done moving, perform experiment aka read sensors, take pics
// Then increment to next motor position
// If position is 8 or greater experiments are done until next cycle.
            if(motComplete == 1) //
            {
                run_experiment();
                motComplete = 0;
            }
        }
    }
    shutdown_experiment();

    return 0;
}



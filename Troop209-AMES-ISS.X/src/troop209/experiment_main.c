#include "system.h"
#include "experiment_main.h"
#include "camera.h"
#include "dateTime.h"
#include "non_pwm_led.h"
#include "SD_support.h"
#include "../troop209hw/SensorDrivers.h"

static void handleSensor();
static void replaceColonWithPeriod(char *temp);

void run_experiment(void) {

    handleSensor();
    
    int mot_inc;
    char filename[40] = {0};
    int stat;
    static const int enableMotor = 3;
    static const int speed = 7;
    static const int motPosition[9]  = {0,512,1024,1536,2048,2560,3072,3584,4096};
    
    for (mot_inc = 0; mot_inc < 8; mot_inc++) {
        // move motor
        stat=checkCarousel(enableMotor, motPosition[mot_inc], speed);
        if (stat == 0)  // functionSUCCESS
        { 
            stat=moveCarousel(enableMotor, motPosition[mot_inc], speed);
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

/**
 * Read sensor, format sensor and write the sensor to SDDataFile
 */
static void handleSensor() {
    extern signed long SNS_Buffer[128];
    
    initSensors();
    readSensors();
    SDDataFile.writeln(SNS_Buffer);
}

static void replaceColonWithPeriod(char *temp)
{
    while(*temp)
    {
        if(*temp == ':')
            *temp = '.';
        
        ++temp;
    }
}

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
    
    int motor_pos;
    char filename[40] = {0};
    for (motor_pos = 0; motor_pos < 8; motor_pos++) {
        // move motor
        
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

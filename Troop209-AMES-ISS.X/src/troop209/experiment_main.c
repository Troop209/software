#include "system.h"
#include "experiment_main.h"
#include "camera.h"
#include "dateTime.h"
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
        
        // LED1 on
        // Camera1
        sprintf(filename, "%s-%s-cam1.jpg", config.label, dateTime.getStamp());
        replaceColonWithPeriod(filename);
        camera.getPix(filename);  
        // LED1 off
        
        // LED2 on
        // Camera2
        sprintf(filename, "%s-%s-cam2.jpg", config.label, dateTime.getStamp());
        replaceColonWithPeriod(filename);
        camera2.getPix(filename);  
        // LED2 off
    }
    
    handleSensor();
}

static void handleSensor() {
    // read sensor
    initSensors();
    readSensors();
    formatSensors(RX_buffer);
    SDDataFile.writeln(RX_buffer);
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

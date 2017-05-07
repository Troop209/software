/* 
 * File:   main.c
 * Author: Joe Pluta (?)
 *
 * Created BEFORE Nov 12, 2016, 3:10 PM
 * 
 * Revision Notes
 * Jan 17, 2017
 * 1. Added #include kernel.h to eliminate compiler warnings (kernel.h new file on Jan 17)
 * Jan 29 2017 merged by Harmon Bhasin
 */
// This version of the main application is configurable
// The program will attempt to read the SD Card to find
//   a config.ini file to drive the rest of the system

#include "experiment_shutdown.h"
#include "experiment_initialize.h"
#include "experiment_main.h"
#include "SD_support.h"
#include "dateTime.h"

int main(void)
{
    initialize_experiment();

    // Delay is in ms and so need to convert time in config 
    int betweenExperimentDuration = config.exp_wait_duration_min * 60 * 1000;

    // MM/dd/YY hh:mm
    DateAndTime expEndDateTime;
    static const int month_pos = 1;
    static const int day_pos = 4;
    static const int year_pos = 7;
    static const int hr_pos = 1;
    static const int min_pos = 4;
    
    char value[2];
    
    value[0] = config.exp_end_date[month_pos];
    value[1] = config.exp_end_date[month_pos + 1];
    expEndDateTime.month = atoi(value);

    value[0] = config.exp_end_date[day_pos];
    value[1] = config.exp_end_date[day_pos + 1];
    expEndDateTime.day = atoi(value);

    value[0] = config.exp_end_date[year_pos];
    value[1] = config.exp_end_date[year_pos + 1];
    expEndDateTime.year = atoi(value);
    
    value[0] = config.exp_end_time[hr_pos];
    value[1] = config.exp_end_time[hr_pos + 1];
    expEndDateTime.hour = atoi(value);

    value[0] = config.exp_end_time[min_pos];
    value[1] = config.exp_end_time[min_pos + 1];
    expEndDateTime.minute = atoi(value);

   
    while ( dateTime.cmp(dateTime.get(), expEndDateTime) >= 0) {
        run_experiment();
        delay(betweenExperimentDuration);
    } 

    shutdown_experiment();
    return 0;
}


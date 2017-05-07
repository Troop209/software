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

    // TODO:  compare time with end time
    while ( TRUE ) {
        run_experiment();
    } 

    shutdown_experiment();
    return 0;
}

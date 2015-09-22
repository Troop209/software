#ifndef DATALOG_H
#define DATALOG_H

#include "system.h"
#include "file.h"

/**
* Wrapper for the Data Log
*/
typedef struct {
    /**
     * Add a value to the data log
     * @param label tag for the data to add. This <code>String</code> preceeds the value.
     * @param value to write to the data log
     */
    void (*add)(String label, unsigned value);
    /**
     * Initializes the data log software module
     */
    void (*init)(void);
    /**
     * Checks data log for issues.
     * @return whether or not the data log file is ok.
     */
    boolean (*isOk)(void);

    FileStream* file;

} DataLog;

extern const DataLog dataLog;

#endif

#ifndef SDCARD_H
#define SDCARD_H

#include "system.h"
#include "../mdd/FSIO.h"
#include "../mdd/HardwareProfile.h"

typedef struct Sdcard Sdcard;
/**
 * Wrapper for SD Card memory
 */
struct Sdcard {
    /**
     * Configures the microcontroller and initializes the filesystem.
     */
    void (*init)(void);
    /**
     * Checks SD card for issues.
     * @return whether or not the SD card was sucessfully initialized
     */
    boolean (*isOk)(void);
};

extern const Sdcard sdcard;

#endif /* SDCARD_H */

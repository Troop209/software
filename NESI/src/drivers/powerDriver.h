#ifndef POWERDRIVER_H
#define POWERDRIVER_H

#include "system.h"


/**
* Wrapper for PowerDriver
*/
typedef struct {
    /**
     * Check if a hardware failure has occured with the powerDriver
     * @return whether or not a hardware failure has occured
     */
    boolean (*isFailure)(void);

    /**
     * Checks status level of powerDriver
     * @return tells you voltage level on low side of Power A's load
     */
    boolean (*status)(void);
    /**
     * Sets the logic value present at the output
     * @param desiredOutputState the digital state to output on the pin (<code>OPEN</code> or <code>CLOSE</code>)
     */
    void (*set)(boolean desiredOutputState);
    /**
     * Turns the powerDriver on
     */
    void (*on)(void);
    /**
     * Turns the powerDriver off
     */
    void (*off)(void);
    /**
     * Initializes the powerDriver software module
     */
    void (*init)(void);

} PowerDriver;


extern const PowerDriver powerDriverA, powerDriverB;

#endif /* POWERDRIVER_H */

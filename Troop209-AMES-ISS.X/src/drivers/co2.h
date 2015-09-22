#ifndef CO2_H
#define CO2_H

#include "system.h"

/**
* Wrapper for CO2 Sensor
*/
typedef struct {
    /**
     * Check if a hardware failure has occured with the CO2 power driver
     * @return whether or not a hardware failure has occured
     */
    boolean (*status)(void);
    /**
     * Read CO2 sensor
     * @return unconverted CO2 voltage
     */
    unsigned (*read)(void);
    /**
     * Take an average sampling of the CO2
     * @param samplesToTake number of samples to take
     * @param interval milliseconds to wait in between samples
     * @return average of the samples taken
     */
    unsigned (*get)(int samplesToTake, int interval);
    
    /**
     * Controls the operation of the CO2 sensor heating element
     * @param desiredState <code>ON</code>/<code>OFF</code> (enable/disable)
     */
    void (*setHeater)(boolean desiredState);
    /**
     * Initializes the CO2 sensor software module
     */
    void (*init)(void);
    /**
     * Turns on the CO2 sub-system and warms the sensor
     */
    void (*on)(void);
    /**
     * Turns the CO2 sub-system off
     */
    void (*off)(void);

} Co2;

extern const Co2 co2;

#endif /* CO2_H */

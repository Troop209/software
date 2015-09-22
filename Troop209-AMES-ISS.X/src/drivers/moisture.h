#ifndef MOISTURE_H
#define MOISTURE_H

/**
 * Wrapper for Moisture Sensors
 */
typedef struct {
    /**
     * Read moisture content of quadrant 1
     * @return moisture value of Q1
     */
    unsigned (*readQ1)(void);
    /**
     * Read moisture content of quadrant 2
     * @return moisture value of Q2
     */
    unsigned (*readQ2)(void);
    /**
     * Read moisture content of quadrant 3
     * @return moisture value of Q3
     */
    unsigned (*readQ3)(void);
    /**
     * Read moisture content of quadrant 4
     * @return moisture value of Q4
     */
    unsigned (*readQ4)(void);
    /**
     * Take an average sampling of the moisture content of quadrant 1
     * @param samplesToTake number of samples to take
     * @param interval milliseconds to wait in between samples
     * @return average of the samples taken
     */
    unsigned (*getQ1)(int samplesToTake, int interval);
    /**
     * Take an average sampling of the moisture content of quadrant 2
     * @param samplesToTake number of samples to take
     * @param interval milliseconds to wait in between samples
     * @return average of the samples taken
     */
    unsigned (*getQ2)(int samplesToTake, int interval);
    /**
     * Take an average sampling of the moisture content of quadrant 3
     * @param samplesToTake number of samples to take
     * @param interval milliseconds to wait in between samples
     * @return average of the samples taken
     */
    unsigned (*getQ3)(int samplesToTake, int interval);
    /**
     * Take an average sampling of the moisture content of quadrant 4
     * @param samplesToTake number of samples to take
     * @param interval milliseconds to wait in between samples
     * @return average of the samples taken
     */
    unsigned (*getQ4)(int samplesToTake, int interval);
    /**
     * Initializes the solenoid software module
     */
    void (*init)(void);

} Moisture;

extern const Moisture moisture;

#endif /* MOISTURE_H */

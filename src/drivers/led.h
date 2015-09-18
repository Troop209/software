#ifndef LED_H
#define LED_H

/**
 * Wrapper for LED using pulse width modulated signal
 */
typedef struct {
    /**
     * Configures the duty cycle of the PWM output signal
     * @param newDutyCycle - the desired duty cycle of the output signal as a positive integer percentage
     *                       (range: <code>MIN_DUTYCYCLE</code> to <code>MAX_DUTYCYCLE</code>)
     */
    void (*dutycycle)(unsigned newDutyCycle);
    /**
     * Initializes the LED software module
     */
    void (*init)(void);
} Led;

extern const Led redLed, blueLed;

#endif /* LED_H */

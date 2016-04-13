#ifndef MOTOR_H
#define MOTOR_H

/**
 * Microcontroller Resource Allocation
 *
 * The following microcontroller resources are used in this module.
 *   - Pins: B (RP16)
 *   - Peripherals: Output Compare 9
 */

#include "system.h"

/**
 * ledX Information
 *
 * ledX are the software wrappers for the NESI LEDs. They use a pulse width
 * modulated signal to control the LED banks. ledR controls the R bank
 * and ledB controls the B bank.
 */
typedef struct {
    /**
     * Configures the duty cycle of the PWM output signal
     * @param newDutyCycle - the desired duty cycle of the output signal as a positive integer percentage
     *                       (range: <code>MIN_DUTYCYCLE</code> to <code>MAX_DUTYCYCLE</code>)
     */
    void (*dutycycle)(Uint16 newDutyCycle);
    /**
     * Initializes the LED software module
     */
    void (*init)(void);
} Motor;

extern const Motor motor;

#endif /* MOTOR_H */

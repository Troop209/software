#ifndef BUTTON_H
#define BUTTON_H

#include "system.h"

typedef enum {
    KEYSTROKE_NONE,
    KEYSTROKE_SINGLE,
    KEYSTROKE_DOUBLE
} Keystroke;

/**
* Wrapper for Button
*/
typedef struct {
    /**
     * Check if the button is pressed down
     * @return digital state of the button
     */
    boolean (*isPressed)(void);
    /**
     * Initializes the button software module
     */
    void (*init)(void);
    /**
     * Disables the detection of button keystrokes
     */
    void (*disableStroke)(void);
    /**
     * Enables the detection of button keystrokes
     */
    void (*enableStroke)(void);
    /**
     * Returns the most recent keystroke detected, and then clears the buffer.
     * @return most recent keystroke detected
     */
    Keystroke (*getStroke)(void);

} Button;

extern const Button button;

#endif /* BUTTON_H */

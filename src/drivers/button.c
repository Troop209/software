#include "system.h"
#include "button.h"

// CN64
static boolean readInput(void)
{
    _TRISE4 = 1; /* configure port as input */
    return !_RE4; /* return the inverse value on the pin */
}

static void initialize(void)
{
    /**
     *  Overview
     *
     *  Timer 5 will be used as the keystroke timeout timer. It will generate
     *  an interrupt X miliseconds after it is started (@setTimeout(X)). The
     *  timer will be started when a change in the button's state is detected,
     *  and stopped automatically at the end of the timeout period. The keystroke
     *  is determined by the number of times the button was pressed during the
     *  timeout period.
     */

    /*  Timer5 Configuration Setup
     *
     *  Timer5 will start off and be started once the button has been pressed
     *  The prescaler will be set to 1:256 so that the period is of a reasonable
     *  value and resolution. The internal system oscillator will be used.
     *  Gating mode and synchronous operation will not be used.
     */
    T5CON = 0x0030;  // disabled, prescaled, internal oscillator
    TMR5 = 0;        // clear the timer

    /*  Timer5 Period Register Setup
     *
     *  The period register contains the number of counts before Timer5 resets.
     *  For 0.5s the count will be half that of the prescaled clocks per second.
     *
     *  Note: The count begins at 0 (rather than 1), so 1 must be subtracted
     *        from the desired value.
     */
    PR5 = (Uint16)((CLOCKS_PER_SEC/256)/2 - 1);  // interrupt 2 seconds later

    /*  Timer5 Interrupt Setup
     *
     *  Interrupts are required, so the interrupt enable bit must be set, and
     *  the interrupt flag must be cleared. Also, the process has normal priority.
     */
    _T5IE = ON;   // enable Timer5 to interrupt
    _T5IF = OFF;  // clear the interrupt flag
    _T5IP = 3;    // set to sub normal priority

    /*  Change-of-State Notification Interrupt Setup
     *
     *  The Change-of-State Notifier interrupts whenever a digital pin changes
     *  its logical state. Interrupts are required, so the interrupt enable bit
     *  must be set, and the interrupt flag must be cleared. The priority of the
     *  detection process is not high, so it is assigned normal priority.
     */
    _CN62IE = ON;      // enable Interrupt on Change 62 to interrupt
    _CNIF   = OFF;     // clear the Interrupt on Change interrupt flag
    _CNIE   = ON;      // enable the Interrupt on Change interrupt
    _CNIP   = 4;       // set the Interrupt on Change interrupt priority to 4


    // read port to clear mismatching (for Change On Interrupt)
    /*  Correct Port Mismatching
     *
     *  The PORT register contains the instantanious value present at the pin,
     *  while the LAT register is latched at clock cycles. To be sure that the
     *  ports match, and prevent a misfire the port is read to clear any
     *  mismatching.
     */
    _LATE4 = _RE4;
}

/*  Keeping Track of Button Presses
 *
 *  The keystroke is determined by counting the number of times that the button
 *  is pressed within the timeout period. pressCount is incremented each time
 *  the button is pressed and released. At the end of the timeout, pressCount is
 *  converted to a keystroke.
 */
static unsigned pressCount = 0;

/* Interrupt on Change interrupt service routine */
_ISR_ _CNInterrupt(void)
{
    /* persistent variable that holds the last value of the mode button */
    static unsigned oldValue = 1; // active low button should start off high

    /* wait a short while before checking value (allow signal to stabilize) and then read the value*/
    delay(50);  // delay 1/2 of 1/10 sec

    unsigned currentValue = _RE4; // store the current value of the button

    /* determine what to do next based on the current state of the Mode button */
    if(currentValue == oldValue) // if no change occurred (false alarm)
    {
        _CNIF = OFF;   // clear the interrupt flag and
        return;        // return (exit)
    }
    else if (currentValue == 0)  // else, if button pressed
    {
        if(!T5CONbits.TON)   // if the timeout has not started
        {
            TMR5 = 0;            // clear the timeout counter
            pressCount = 0;      // clear the stroke count
            _T5IE = ON;          // enable Timer5 to interrupt
            T5CONbits.TON = ON;  // begin the timeout sequence
        }
    }
    else
    {
        /* if the state changed, but the button is not down, it was released */
        ++pressCount; // accumulate the number of button taps
    }

    /* now that we're done, the current value is now the old value */
    oldValue = currentValue;

    /* clear the interrupt flag so that the button can be detected if pressed */
    _CNIF = OFF;

} // CNInterrupt end

/**
 * Stores the most recent keystroke that was detected.
 */
static Keystroke latestKeystroke = KEYSTROKE_NONE;

/* Get the most recent keystroke that was detected and then clear the buffer. */
static Keystroke getKeystroke(void)
{
    static Keystroke temp = KEYSTROKE_NONE;

    temp = latestKeystroke;
    latestKeystroke = KEYSTROKE_NONE;
    return temp;
}

static void enableKeystroke(void)
{
    _CN62IE = ON; // enable Interrupt on Change 62 to interrupt
    _CNIF = OFF;  // clear the Interrupt on Change interrupt flag
    _CNIE = ON;   // enable the Interrupt on Change interrupt
}

static void disableKeystroke(void)
{
    _CN62IE = OFF;        // disable Interrupt on Change 62 from interrupting
    _CNIF = OFF;          // clear the Interrupt on Change interrupt flag
    _CNIE = OFF;          // disable the Interrupt on Change interrupt
    _T5IE = OFF;          // disable Timer5 from interrupt
    _T5IF = OFF;          // clear the interrupt flag
    TMR5 = 0;             // clear the timeout counter
    T5CONbits.TON = OFF;  // stop the timeout sequence

    latestKeystroke = KEYSTROKE_NONE;
}

/**
 *  Timeout to detect button keystroke.
 */
_ISR_ _T5Interrupt(void)
{
    /* stop and reset the timer, now that we're done using it */
    T5CONbits.TON = OFF;
    TMR5 = 0;

    /* assign the keystroke based on the number of times the button was pressed */
    switch(pressCount)
    {
        case 0:
            latestKeystroke = KEYSTROKE_NONE;
            break;
        case 1:
            latestKeystroke = KEYSTROKE_SINGLE;
            break;
        case 2: default:
            latestKeystroke = KEYSTROKE_DOUBLE;
            break;
    }

    /* reset count */
    pressCount = 0;


    _T5IE = OFF; // disable Timer5 from interrupt
    _T5IF = OFF; // clear the interrupt flag

} // T5Interrupt end

const Button button = {isPressed:readInput, init:initialize, getStroke:getKeystroke, enableStroke:enableKeystroke, disableStroke:disableKeystroke};

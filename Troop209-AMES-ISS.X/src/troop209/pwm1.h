#ifndef PWM_H
#define PWM_H

// Defines different timer selection option as defined from Chap 14 of Data Sheet (page )
#define PWM_TIMER_SELECTION_TIMER1  4 // 
#define PWM_TIMER_SELECTION_TIMER2  0 // 
#define PWM_TIMER_SELECTION_TIMER3  1 // 
#define PWM_TIMER_SELECTION_TIMER4  2 // 
#define PWM_TIMER_SELECTION_TIMER5  3 //
#define PWM_TIMER_SELECTION_SYSTEM  8 //

/**
 * Wrapper for raw pulse width modulated signal
 */
typedef struct {
    /**
     * Configures the period and duty cycle of the PWM output signal
     * @param dutycycle - the desired duty cycle of the output signal as a positive integer percentage
     * @param period - the period of the PWM in clock cycles
     */
    void (*turnOn)(unsigned period, unsigned dutycycle);
    /**
     * Turn off the PWM
     */
    void (*turnOff)();
    /**
     * Initializes the PWM software module
     */
    void (*init)();
} PWM;

extern const PWM pwm1;

#endif /* PWM_H */

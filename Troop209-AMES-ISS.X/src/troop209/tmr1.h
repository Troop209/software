#ifndef TIMER1_H
#define TIMER1_H

// From Chap 11 of Data Sheet
#define TIMER_PRE_SCALE_256 = 3 // 11
#define TIMER_PRE_SCALE_64 = 2  // 10
#define TIMER_PRE_SCALE_8 = 1   // 01 
#define TIMER_PRE_SCALE_1 = 0    // 00 

/**
 * Wrapper for Timer1 using prescaled internal clock (FOSC/2)
 */
typedef struct {
    /**
     * Configures the prescale and preiod of the TIMER1 output signal
     * @param prescale - the desired prescale (0-3, 1/8/64/256)
     * @param period - the period of the TIMER1 in prescaled clock cycles
     */
    void (*start)(unsigned prescale, unsigned period);
    /**
     * Turn off the TIMER1
     */
    void (*stop)();
} TIMER1;

extern const TIMER1 timer1;

#endif /* TIMER1_H */

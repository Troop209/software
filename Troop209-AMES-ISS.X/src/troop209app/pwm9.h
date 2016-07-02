#ifndef PWMA_H
#define PWMA_H

/**
 * Wrapper for raw pulse width modulated signal
 */
typedef struct {
    /**
     * Initializes the PWM software module
     */
    void (*init)();
    
    void (*setJog)(int jog);
    
    void (*setDutyCycle)(int dutyCycle); 
    
    void (*setPeriod)(int period); 
    
} PWMA;

extern const PWMA pwma9;

#endif /* PWMA_H */

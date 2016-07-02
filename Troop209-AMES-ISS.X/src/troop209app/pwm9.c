#include "../core/system.h"
#include "pwm9.h"

/* Initializes the OC module */
static void initRawPwm9() {
    T4CON = 0; // clear everything out including t32 "Gang counter" bit
    T5CON = 0; // clear everything out
    T5CON = 0x0030; // TImer 5 enable, /256, interrnal 16G clock
    nop(); // let hardcware catch up to software (1 instruiction cycle))
    PR5 = 3124; // 20 Hz POeriod
    nop(); // and 1 extra 
    T5CON = 0x8030; // TImer 5 enable, /256, interrnal 16G clock
    nop(); // let hardcware catch up to software (1 instruiction cycle))
    nop(); // and 1 extra 

    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR8 = 35; //Select oc 9  as RP16
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers

    OC9CON1 = 0x1C08;
    nop();
    OC9CON2 = 0x008F;
    nop();
    OC9RS = 24000;
    OC9R = 56000;
    OC9CON1 = 0x1C0A; // All set up. Now Go
    nop();
    return;
}

static void SET_PWM9_Period(int period) {
    PR5 = period; // write lower word to 7. andED off upper 16 bits
}

static void SET_PWM9_DutyCycle(int dutycycle) {
    OC9RS = dutycycle;
}

static void SET_PWM9_JOG(int jog) { // add an offset to the current motor position
    // intended to do small move until Align switch makes
    OC9R = OC9R + jog;
}


const PWMA pwma9 = {init : initRawPwm9,
    setJog : SET_PWM9_JOG, setDutyCycle : SET_PWM9_DutyCycle, setPeriod : SET_PWM9_Period};

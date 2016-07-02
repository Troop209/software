#include "../core/system.h"
#include "tmr5.h"
#include "../norm/PIC24_Peripherals.h"

int main(int argc, char** argv) {
    // initialize all modules

    system.init();
    system.on6volt();
    int period = 3125;
    int dutcycl = 24000;
    int pos = 0;
    int position = 16000;
    int mot = 5000;
    int PWM_Positions[10] = {16000, 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 16000};
    long Mdelay  = 10000      ;

    PWM9_init();
    SET_PWM9_Period(period);
    SET_PWM9_DutyCycle(dutcycl);

    while (mot > 0) {
        pos = 0;
        while (pos < 9) {
            position = PWM_Positions[pos];
            SET_PWM9_DutyCycle(position);
            delay(Mdelay);
            pos++;
        }
        --mot;
    }
    return (TRUE);
}

void ServoInit() {
    int dutcycl = 24000;
    int period = 3125;

    PWM9_init();
    SET_PWM9_Period(period);
    SET_PWM9_DutyCycle(dutcycl);
}

void ServoRun() {
    int pos = 0;
    int position = 0;
    int PWM_Positions[10] = {16000, 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 16000};
    long Mdelay = 10000;
    while (pos < 9) {
        position = PWM_Positions[pos];
        SET_PWM9_DutyCycle(position);
        delay(Mdelay);
        pos++;
    }

}


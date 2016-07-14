/**
 * Non PWM LED (aka standard LED)
 * Copyright (C) 2016 Pathway Council - Troop 209
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "system.h"
#include "non_pwm_led.h"

/**
 * initPwm1() Information
 *
 * Standard LED that is not tied with PWM, basis using LED code
 */
static void toggleLED1(Boolean desiredOutputState) {
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR4 = 0x0000; /* clear RP8 and RP9 so Port drives pins */
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers
    _TRISB9 = 0; /* configure port as output */
    _RB9 = desiredOutputState; /* set the output */
}

static void setOnLED1(void) {
    toggleLED1(TRUE); /* set the output to be open */
}

static void setOffLED1(void) {
    toggleLED1(FALSE); /* set the output to be closed */
}

static void toggleLED2(Boolean desiredOutputState) {
    __builtin_write_OSCCONL(OSCCON & 0xBF); // unlock Peripheral Pin Select Registers
    RPOR4 = 0x0000; /* clear RP8 and RP9 so Port drives pins */
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock Peripheral Pin Select Registers
    _TRISB8 = 0; /* configure port as output */
    _RB8 = desiredOutputState; /* set the output */
}

static void setOnLED2(void) {
    toggleLED2(TRUE); /* set the output to be open */
}

static void setOffLED2(void) {
    toggleLED2(FALSE); /* set the output to be closed */
}

/**
 * ledX Information
 *
 * ledX are the software wrappers for the NESI LEDs. They use a pulse width
 * modulated signal to control the LED banks. ledR controls the R bank
 * and ledB controls the B bank.
 */
const NonPWMLed led1 = {on : setOnLED1, off : setOffLED1},
led2 = {on : setOnLED2, off : setOffLED2};

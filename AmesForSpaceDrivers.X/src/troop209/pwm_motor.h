/**
 * PWM Motor 
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

#ifndef PWM_MOTOR_H
#define PWM_MOTOR_H

/**
 * Microcontroller Resource Allocation
 *
 * The following microcontroller resources are used in this module.
 *   - Pins: B8 (RP8), B9 (RP9)
 *   - Peripherals: Output Compare 1 and 2
 */

#include "system.h"

/**
 * Using NESI LED program as the base but no PWM and as such it is standard LED
 */
typedef struct {
    void (*init)(void);
    int (*turn)(int);
} Motor;

extern const Motor motor;

#endif /* PWM_MOTOR_H */

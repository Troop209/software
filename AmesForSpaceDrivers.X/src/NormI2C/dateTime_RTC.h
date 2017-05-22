/* 
 * File:   dateTime_RTC.h
 * Author: Norman McFarland
 *
 * Created on September 6, 2016, 11:02 AM
 */

#ifndef DATETIME_RTC_H
#define	DATETIME_RTC_H

#ifdef	__cplusplus
extern "C" {
#endif
/**
 * NESI DateTime Module License
 * Copyright (C) 2013 Mickie Byrd
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


/**
 * Microcontroller Resource Allocation
 *
 * The following microcontroller resources are used in this module.
 *   - Pins: C12 (Clock Input), C15 (Clock Out)
 *   - Peripherals: RTCC (Real Time Clock and Calendar),
 *                  Timer 4 (millisecond count- currently unused)
 */

/**
 * Change Log
 *
 * 12/19/2013 - Mickie Byrd
 *   Added .toStamp() and .parseStamp() to handle time stamp string processing.
 *   Added .new() member to be used for initializing DateAndTime variables.
 * 12/24/2013 - Mickie Byrd
 *   Added .compare() to be used for comparing two DateAndTime objects.
 * 12/26/2013 - Mickie Byrd
 *   Changed .new() by adding parameters that are used to initialize returned
 *       DateAndTime object.
 *   Added .add() and .sub() to be used to add and subtract DateAndTime objects.
 *   Refactored .compare() to .cmp() for consistency.
 * 12/27/2013
 *   Modified .new() to create DateAndTime without normalizing value.
 *   Added .to() which does what .new() used to do.
 */

/**
 * TODO
 *
 * Things left to do:
 *   comments - functions and code
 *   refactoring - simplification and condensing
 */

#include "system.h"
#include "datetime.h"

//extern const DateTime dateTime;    
// ** JDP1 ** void getTimeStamp(char *RTC_I2C_TimeStamp)  ;
void getRTCTimeStamp(char *RTC_I2C_TimeStamp)  ;
int setI2C2_RTCTime(char *RTC_I2C_TimeStamp) ;
int getI2C2_RTCTime(char *RTC_I2C_TimeStamp) ;
void setPIC_RTCTime(char *RTC_I2C_TimeStamp)    ;





#ifdef	__cplusplus
}
#endif

#endif	/* DATETIME_RTC_H */


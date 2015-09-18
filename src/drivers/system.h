#ifndef SYSTEM_H
#define SYSTEM_H

/*  Include the PIC24FJ256GB106 Device Header
 *
 *  A Microchip PIC24FJ256GB106 microcontroller as its central intelligence.
 *  In order for code to successfully compile, the device header file must
 *  be included. This header file is provided by the manufacture and contains
 *  all of the required declarations, definitions, and macros that are
 *  necessary for configuring the PIC24FJ256GB106 microcontroller using the
 *  C-programming language.
 */
#include "../dev/p24FJ256GB106.h"

/*  Interrupt Service Routine Declaration Macro
 *
 *  Interrupt service routines for PIC microcontrollers are declared a little
 *  differently than normal functions are. These void returning functions have
 *  special compiler attributes that dictate their definition. Since none of
 *  the interrupt service routines used with E-Clock ™ will utilize the advanced
 *  declaration options, a generic configuration will be defined and used.
 */
#define _ISR_ void __attribute__((interrupt, no_auto_psv))

/*  Processor Speed Constant
 *
 *  When doing various time dependent calculations and configuring, the speed
 *  at which instructions are being executed must be known. This is especially
 *  true when configuring peripherals that are timed directly from the CPU's
 *  clock (ex. PWM for lighting).
 */
#define FOSC            (32000000LL)
#define FCY             (FOSC/2)    /* CPU runs at half speed.        */
#define CLOCKS_PER_SEC  (FCY)       /* CPU clocks per second (instructions per second) */

/*  Simple Delays
 *
 *  It is sometimes useful to pause, or delay the execution of code. The manufacturer
 *  time library will be used in order to provide accurate delays.
 */
#include <libpic30.h>

#define delay_us(x) __delay_us(x) /* __delay32(((x*FCY)/1000000LL)) */
#define delay_ms(x) __delay_ms(x) /* __delay32(((x*FCY)/1000LL))    */

#define pause(x)        delay_ms(x)
#define wait(x)        delay_ms(x)
#define delay(x)        delay_ms(x)
#define delayCycles(x)  __delay32(x)

/**  round() Macro Information
  *
 *  @Purpose
 *     Rounds a number to the nearest integer.
 *  @Parameters
 *     Number to be rounded.
 *  @Returns
 *     Closest integer number.
 */
#define round(x)    ((int)((x) + 0.5))

/**  boolean Type Information
 *
 *  A boolean is defined to be either 0 (FALSE) or 1 (TRUE). This enumeration
 *  offers several boolean alternatives.
 */
typedef enum {
    OFF    = 0b0,   ON      = 0b1,        // True != 0xFFFF
    FALSE  = OFF,   TRUE    = ON,
    NO     = OFF,   YES     = ON
} boolean;

/**  IntX Information
 *
 *  The following macros are used to declare integers having a width of "X"
 *  bits for the TMS570 microprocessor. UintX are unsigned and SintX are
 *  signed. IntX defaults to the default signing of the compiler.
 */
#define Int     int
#define Sint    signed int
#define Uint    unsigned int

#define Int8    char
#define Sint8   signed char
#define Uint8   unsigned char

#define Int16   short
#define Sint16  signed short
#define Uint16  unsigned short

#define Int32   long
#define Sint32  signed long
#define Uint32  unsigned long

#define Int64   long long
#define Sint64  signed long long
#define Uint64  unsigned long long

typedef Uint8   Byte;
typedef Uint16  Hword;
typedef Uint32  Word;
typedef Uint64  Dword;

typedef char*   String;

#define NullPtr ((void*)0)

/**
 * Wrapper for System
 */
typedef struct {

    /**
     * Initializes the system
     */
    void (*init)(void);

    /**
     * Turns 6volt enable on
     */
    void (*on6volt)(void);

    /**
     * Turns 6volt enable off
     **/
    void(*off6volt)(void);

} System;

extern const System system;

#endif  /* this is the end of the SYSTEM_H conditional compilation block     */

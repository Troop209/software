/* 
 * File:   PIC24_Peripherals.h
 * Author: Norman McFarland
 *
 * Created on February 5, 2016, 12:23 PM
 */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *         TOPIC                        approx line #
 *       Pulse Width Modulation (PWM)        15
 *       Read Signal Period ()RSP)          125
 *       Read Analog Channel 10             170
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 */

  
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *       Pulse Width Modulation (PWM)
 *
 * This driver provides a pulse width modulation output (PWM) with 32 bit resolution 
 *  
 * The timing is driven directly by system clock (Fcy) at 16,000,000,000 hertz
 * This gives resolutions down to 62.5 nanoSeconds
 * 
 * Our primary use of the PWM output is for a servo motor
 * 
 * Servo motors are typically 1/2 turn or -90 degrees to +90 degrees.
 * These servos need a pulse width modulated signal
 *   |-----|______________________________________________________________|-----|____________________
 *   |     |      pulse width (seconds)                                   
 *   |            period      (seconds)                                   |
 *   |            period = 1/frequency
 *   |            duty cycle=  pulse width/period
 * 
 * The "standard" interface is 1.0 mS for -90 degrees, 1.5mS for 0 degrees and 2.0mS for +90 degrees
 * The period (time between these pulses is about 20 hertz or 50 mSec
 * The pulse width modulation driver is set up for 256 steps
 *   of 3.9 uSec per step giving a positional resolution of 0.7 degrees per step.
 * 
 * Servo motors can also be purchased that are full turn (360 degrees) or multi turn n * 360 degrees
 * These servos typically use the same 1.0 mSec to 2.0 mSec signal meaning that the 256 steps now n*0.7 degrees so a
 *    1 full turn ( -180 to +180  degrees) is 1.4 degrees per pulse
 *    2 full turn ( -360 to +360  degrees) is 2.8 degrees per pulse
 *    4 full turn (-1440 to +1440 degrees) is 5.6 degrees per pulse
 *    and so on
 *   
 * To compensate, many manufactures use 4096 steps of resolution for the multi turn units
 *          4 0 9 6 pulse from 1.0 mSec to 2.0 mSec
 *    1 full turn ( -180 to +180  degrees) is 0.0875 degrees per pulse
 *    2 full turn ( -360 to +360  degrees) is 0.1750 degrees per pulse
 *    4 full turn (-1440 to +1440 degrees) is 0.3500 degrees per pulse
 *    and so on
 * 
 *   These 4096 steps requires a higher base frequency clock to generate this resolution
 * 
 * A third type of servo is the continuous motion servo
 * These still use the 1.0 t0 2.0 mS pulse but the usage is much different
 * 1.5 mS pulse means no motion
 * from 1.5 to 2.0 turns motor 1 direction at increasing speeds while
 * from 1.5 to 1.0 turns motor other direction at increasing speeds.
 * 
 * Note that the continuous  motor CONTINUES to ROTATE until pulse set back to 1.5 mS
 * WE DO NOT WANT THIS TYPE OF SAERVO FOR OUR PROJECT!
 *
 *      IMPLEMENTATION NOTES:
 * 
 * In reviewing the PIC24 on-chip peripherals Output Compare 7 and 8 were going to be used in a 32 bit wide PWM format
 *   but was learned that in 32 bit mode the pulse width had to be greater than 65536 counts and all of our outputs were under 64000
 * 
 * The alternative was to create the frequency duty cycle in one peripheral (Timer 5) and
 *   use the Timer 5 output to trigger the Output COmpare 9 for the pulse width
 * This is what was done
 * 
 * Timer 5 is configured to divide the 16,000,000p Hertz clock by 256 to a 62500 base frequency
 *         in addition The Timer 5 counter will be set to divide by a number to derive a 50 mSec/20 Hz to 50 mSec/20Hz period
 *         16000000/256/50  =   1250 counts for 20 mSec/50 Hz
 *         16000000/256/20  =   3125 counts for 50 mSec/20 Hz
 * The output of Timer 5 is a rising edge pulse at above frequency/period
 * 
 * This pulse triggers the rising edge of the pulse from OC9.
 *  - - - - - -
 * 
 * Output COmpare 9 is configured out output one pulse every time that a rising edge is received from Timer 5
 * 
 *Pulse Times can be calculated by dividing the desired pulse width (or period) 
 *  by the inverse of 16,000,000 or .0000000625 Sec (62.5 nS)
 * 
 * So a 1.0 mSec pulse is then .0010/.0000000625 or 16,000 counts
 * So a 1.5 mSec pulse is then .0015/.0000000625 or 24,000 counts
 * So a 2.0 mSec pulse is then .0020/.0000000625 or 32,000 counts
 * 
 * Note that this resolution is probably 4 times what the servo motor is likely to resolve
 *  SO we will agree that any value passed for a pulse width will be evenly divisible by 4. This will compensate for limited motor resolution.
 *  
 * So full counter-clockwise (-nnn degrees) is a value of 16000 into the OC9 OC9R Register and 
 * So full         Clockwise (+nnn degrees) is a value of 323000 into the OC9 OC9R Register and
 * So the   middle position is ( 0 degrees) is a value of 24000 into OC9 OC9R register
 * 
 * There is another concern that the cylinder may not go to the exact location needed for the picture.
 * Switches have been included to give feedback of the actual cylinder position
 * 
 * There may be a need to make small mves of the cylinder to position the cylinder for the pictuire (switch closed)
 * 
 * The PWM9_Jog function allows the user to move +- nnn steps from CURRENT position
 * This is a signed value and can move the motor either clockwise or counter clockwise
 * 
 * int      PWM9_init(void)                       // initializes the PWM peripherals                    ;
 * int      SET_PWM9_Period(long int Period)        // Sets the PWM Period in PWM Peripherals             ;
 * int      SET_PWM9_DutyCycle(long int dutycyc)    // Sets the PWM Dutry cycle in in PIC24 Peripherals   ;
 * void     SET_PWM9_Jog(int jog)                                               ;
 * 
 * 
 * All four functions are set up to return a status, but all return error free 0 status at this time.
 */


// Pulse Width Modulation Output. (PWM) 16 bit resolution

int      PWM9_init(void)                                                     ;
int      SET_PWM9_Period(long int Period)                                    ;
int      SET_PWM9_DutyCycle(long int dutycyc)                                ;
void     SET_PWM9_Jog(int jog)                                               ;
// long int SET_PWM9_Get_Amps(long int dutycyc)                                 ;


 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
        READ SIGNAL PERIOD (RSP)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Processor resources were better utilized to measure signal period rather than signal frequency
 * Period is exactly equal to 1/Frequency
 * 
 * HW measures time from rising edge of signal to next rising edge of signal
 *  ("rising edge" is the transition from logic 0 to logic 1 and falling edge is transition from logic 1 to logic 0)
 *  Falling edge is ignored by this module (but can be reprogrammed to measure that)
 * 
 * The HW module has a free running (never stopping) counter. The counter runs 
 *  from 0x00000000 to 0xFFFFFFFF then "rolls over" to 0 and continues 
 * 
 * On each signal rising edge, the hardware captures the current counter value. 
 * 
 * When called, this driver first clears all the old information in the Hardware FIFO and in the local storage array.
 * Driver then waits for a new rising edge value to be captured by the HW, and stores that in the first data array position (0).
 * Driver then waits for 4 more reading values, placing each in the array at successive locations.
 * 
 * After getting the fifth reading, the driver calculates the width o(in counts) of each reading by
 *  subtracting reading 2 from 1, 3 from 2, 4, from 3, and 5 from 4. The differences are written back into the array
 *  the differences are also added together to get a sum total, thjen divided by 4 to get the average value of the 4 readings.
 * 
 * The average count reading is then converted to a time reading by multiplying the count timers the clock signal width (62.5nS)
 * The resultant value is returned to the calling program as a long integer (32 bits).
 * 
 * Execution of this driver will require slightly more than 4 periods of the incoming signal. For faster signals, that s not a problem,
 *   but for slower signals caution should be exercised in what else may be happening in the time interval.
 * 
 * Note that nano seconds  are billionths  of a second  0.000000001
 *           micro seconds are millionths  of a second  0.000001
 *           milli seconds are thousandths of a second  0.001
 * 
 * SW Notes: AT end of driver, had trouble with divisions. please read notes on solution
 * 
 * SW Options: If division issues resolved (see immediately above), then can change driver 
 *  so user passes how many samples they want to make (and wait for)
 */

// Read SIgnal Period (to determine signal frequency). results is in nano seconds
int      RSP78_init (void)                                                   ;
long int RSP78_Read_Sig_Period(void)                                         ;



// A/D channel 10 init, read, and sample
 // void  A2D_c10_init (void)                               ;
void   Analog_init(void)                                 ;
Uint16 A2D_c10_read(void)                                ;
Uint16 A2D_c10_sample(int samplesToTake, int interval)   ;

/*
 *  The Servo motor is configured with a current sensing resistor in its ground lead.
 *  This 1.2 ohm resistor will develop a voltage (referenced to ground) proportional to motor current
 *  The formula for the voltage created is V = I * R where V=voltage, I=Current, and R = Resistance
 *  so some examples are given below.
 * 
 *  The non-=ground side of the resistor is connected to NESI board JP-8 pin 5
 *  (This was an available pin on the connector that was an Analog input on the PIC 24 )
 *  The pic24 Pin can be configured as analog in put channel 10 (c10).
 *  The A/D converter converts voltage on an input pin to a 10 bit count
 *  mathmatically:  Vin/(Vcc-Gnd) * 1024  OR Vin (3.3V-0,0V) * 1024
 * 
 * We can now calculate the A/D counts for the input voltages across the current sense resistor
 *  0.050 A    50 mA      at 1.2 Ohms   0.060 V     ~18 counts +- 1 count
 *  0.100 A   100 mA      at 1.2 Ohms   0.120 V     ~37 counts +- 1 count
 *  0.150 A   150 mA      at 1.2 Ohms   0.180 V     ~56 counts +- 1 count
 *  0.200 A   200 mA      at 1.2 Ohms   0.240 V     ~75 counts +- 1 count
 *  0.250 A   250 mA      at 1.2 Ohms   0.300 V     ~93 counts +- 1 count
 *  0.300 A   300 mA      at 1.2 Ohms   0.360 V    ~112 counts +- 1 count
 *  0.350 A   350 mA      at 1.2 Ohms   0.420 V    ~130 counts +- 1 count
 * 
 * The above works out to 0.3714 milliamperes per A/D count
*/

long int RSP_OneBillion =   1000000000  ;   // for calculation frequency from period with nanosecond resolution


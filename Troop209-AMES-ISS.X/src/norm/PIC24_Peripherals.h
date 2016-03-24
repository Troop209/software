/* 
 * File:   PIC24_Peripherals.h
 * Author: Norman McFarland
 *
 * Created on February 5, 2016, 12:23 PM
 */

// Read SIgnal Period (to determine signal frequency). results is in nano seconds
int      RSP78_init (void)                                                   ;
long int RSP78_Read_Sig_Period(void)                                         ;

// Pulse Width Modulation Output. (PWM) 32 bit resolution
int      PWM9_init(void)                                                     ;
int      SET_PWM9_Period(long int Period)                                    ;
int      SET_PWM9_DutyCycle(long int dutycyc)                                ;
void     SET_PWM9_Jog(int jog)                                               ;
long int SET_PWM9_Get_Amps(long int dutycyc)                                 ;

// A/D channel 10 init, read, and sample
 void  A2D_c10_init (void)                               ;
Uint16 A2D_c10_read(void)                                ;
Uint16 A2D_c10_sample(int samplesToTake, int interval)   ;

long int RSP_OneBillion =   1000000000  ;   // for calculation frequency from period with nanosecond resolution

typedef struct {
    /**
     * Read resistive sensors content of quadrant 1
     * @return resistive sensors value of Q1
     */
    Uint16 (*init_C10)(void);
    /**
     * Read resistive sensors content of quadrant 1
     * @return resistive sensors value of Q1
     */
    Uint16 (*get_C10)(void);
   /**
     * Take an average sampling of the resistive sensors content of quadrant 1
     * @param samplesToTake number of samples to take
     * @param interval milliseconds to wait in between samples
     * @return average of the samples taken
     */
    Uint16 (*sample_C10)(int samplesToTake, int interval);
  
} ReadServoAmps ;

extern const ReadServoAmps readServoAmps ;
  


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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
        Pulse Width Modulation (PWM)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * This driver provides a pulse width modulation output (PWM) with 32 bit resolution 
 *  
 * The timing is driven directly by system clock (Fcy) at 16,000,000,000 hertz
 * This gives resolutions down to 62.5 nanoSeconds
 * 
 * Our primary use of the PWM output is for a servo motor
 * 
 * Servo motors are typically 1/2 turn or -90 degrees to +90 degrees.
 * These servos need a pulse width modulated signal
 * The "standard" interface is 1.0 mS for -90, 1.5mS for 0 degree and 2.0mS for +90 degrees
 * The period (time between these pulses is about 20 hertz or 50 mSec
 * Often times, the pulse width modulation driver is set up for 256 steps
 *   of 3.9 uSec per step giving a positional resolution of 0.7 degrees per step.
 * 
 * Servo motors can also be purchased that are full turn (360 degrees) or multi turn n * 360 degrees
 * These servos typically use the same 1.0 to 2.0 signal meaning that the 256 steps
 *   now are more degrees of motion and less per step resolution.
 * To compensate, many manufactures use 4096 steps of resolution for the multi turn units
 * This requires a higher base frequency clock to give this resolution
 * 
 * In reviewing the PIC24 internal modules, it was decide to not "waste" an internal module
 *   for a pre-scaler, instead to direct drive from the system clock (Fcy).
 * TImes can be calculated by dividing the desired pulse width (or period) 
 *  by the inverse of 16,000,000 or .0000000625 Sec (62.5 nS)
 * 
 * So a 1.0 mSec pulse is then .0010/.0000000625 or 16,000 counts
 * So a 1.5 mSec pulse is then .0015/.0000000625 or 24,000 counts
 * So a 2.0 mSec pulse is then .0020/.0000000625 or 32,000 counts
 * 
 * Note that this resolution is probably 4 times what the motor is likely to resolve
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
 * Three functions are provided for this PWM solution
 *  PWM_init    initializes Output Comparators 7 & 8 as a single 32 bit PWM unit
 *  SET_PWN_Period (long int) sets the period of the output pulse in 16M counts
 *  SET_PWN_Duty_Cycle (long int) sets the period of the output pulse width (high time) in 16M counts
 * 
 * All three functions are set up to return a status, but all return error free 0 status at this time.
 */


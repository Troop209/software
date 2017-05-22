/* 
 * File:   RadiationSensorDriver.h
 * Author: Norman McFarland
 *
 * Created on August 31, 2016, 11:08 AM
 */

#ifndef RADIATIONSENSORDRIVER_H
#define	RADIATIONSENSORDRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif


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
int      initRadiation (void)   ;
long int readRadiation(void)    ;

int      initRad (void)   ;
long int readRad(void)    ;
#ifdef	__cplusplus
}
#endif

#endif	/* RADIATIONSENSORDRIVER_H */


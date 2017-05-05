/* 
 * File:   SensorDrivers.h
 * Author: Norman McFarland
 *
 * Created on September 1, 2016, 2:38 PM
 * * 
 * Changes to file:
 * Rev 1 set January 5, 2017
 * 1.1 1.1  change 'formatSensors90 from char pointer to byte pointer 
 */

#ifndef SENSORDRIVERS_H
#define	SENSORDRIVERS_H

#ifdef	__cplusplus
extern "C" {
#endif


int initSensors (void)      ;
int readSensors (void)      ;
int formatSensors(Byte* Ptr)  ;


#ifdef	__cplusplus
}
#endif

#endif	/* SENSORDRIVERS_H */

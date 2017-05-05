/* 
 * File:   TempHumidPresDrivers.h
 * Author: Norman McFarland
 *
 * Created on August 29, 2016, 1:59 PM
 * 
 * Changes to file:
 * Rev 1 set Dec 2016
 * 1.1  add diagRecord function prototype
 */

#ifndef TEMPHUMIDPRESDRIVERS_H
#define	TEMPHUMIDPRESDRIVERS_H

// "External" calls:
int  initTHP (void)         ;
int  readTHP (void)         ;
void diagRecord(void)      ;
#define THP_Addr     0X77

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* TEMPHUMIDPRESDRIVERS_H */


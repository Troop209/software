/* 
 * File:   Encoder2.h
 * Author: Norman McFarland
 *
 * Created on January 1, 2017, 1:36 PM
 * 
 * Rev 1 Feb 9, 2017 Norman McFarland
 * 1.1 Add monitorEncoder function prototype
 * 1.2 remove calibrateEncoder variables in call 
 */

#ifndef ENCODER2_H
#define	ENCODER2_H

void initEncoder(void)              ;
int  readEncoder(void)              ;   // return rotation in 1/4096 steps of full circle
int  readEncoderIrq(void)           ;   // return rotation in 1/4096 steps of full circle
int  monitorEncoder(int mSdelay)    ;
int  calibrateEncoder(void)         ;


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* ENCODER2_H */


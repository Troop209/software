/* 
 * File:   Analog2Digial.h
 * Author: Norman McFarland
 *
 * Created on July 15, 2016, 2:14 PM
 */

#ifndef ANALOG2DIGIAL_H
#define	ANALOG2DIGIAL_H

#ifdef	__cplusplus
extern "C" {
#endif
void initA2D(void)  ;
Uint16 readQ1(void) ;   // Q1 is A/D Channel 0
Uint16 readQ2(void) ;   // Q2 is A/D Channel 1
Uint16 readQ3(void) ;   // Q3 is A/D Channel 2
Uint16 readQ4(void) ;   // Q4 is A/D Channel 3
Uint16 readC10(void) ;  // A/D Channel 10




#ifdef	__cplusplus
}
#endif

#endif	/* ANALOG2DIGIAL_H */


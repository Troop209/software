/* 
 * File:   multiMotorDrivers.h
 * Author: Norman McFarland
 *
 * Created on May 19, 2017, 4:55 PM
 */

#ifndef MULTIMOTORDRIVERS_H
#define	MULTIMOTORDRIVERS_H

#ifdef	__cplusplus
extern "C" {
#endif

void    initStepper(void)  ;
int     checkMotor(void)    ;
int     multiMotorMove(int AppPattern, int AppAngle, int AppSpeed)  ;
int     moveCarousel(int pattern, int angle, int speed)             ;
int     checkCarousel(int pattern, int angle, int speed)            ;


#ifdef	__cplusplus
}
#endif

#endif	/* MULTIMOTORDRIVERS_H */


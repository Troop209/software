/* 
 * File:   Gyroscope.h
 * Author: Norman McFarland
 *
 * Created on April 18, 2016, 5:12 PM
 */

#ifndef GYROSCOPE_H
#define	GYROSCOPE_H

int GYR_Configure1 (void)       ;
int GYR_Configure2 (void)       ;
int GYR_Configure3 (void)       ;
int GYR_ReadStatus (void)       ;
int GYR_ReadValues (void)       ;
int GYR_Diag_RdCfgs (void)      ;
int GYR_Diag_RdIrqs (void)      ;

#define GYR_Addr     0X6B       // GYR I2C Address

#endif



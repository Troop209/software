/* 
 * File:   Gyroscope.h
 * Author: Norman McFarland
 *
 * Created on April 18, 2016, 5:12 PM
 */

#ifndef MAGNETOMETER_H
#define	MAGNETOMETER_H

int MAG_Configure1 (void)       ;
int MAG_Configure2 (void)       ;
int MAG_Configure3 (void)       ;
int MAG_ReadStatus (void)       ;
int MAG_ReadValues (void)       ;
int MAG_Diag_RdCfgs (void)      ;
int MAG_Diag_RdIrqs (void)      ;

#define MAG_Addr     0X1D       // MAG I2C Address

#endif



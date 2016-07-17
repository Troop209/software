/* 
 * File:   Gyroscope.h
 * Author: Norman McFarland
 *
 * Created on April 18, 2016, 5:12 PM
 */

#ifndef ACLOSCOPE_H
#define	ACLOSCOPE_H

int ACL_Configure1 (void)       ;
int ACL_Configure2 (void)       ;
int ACL_Configure3 (void)       ;
int ACL_ReadStatus (void)       ;
int ACL_ReadValues (void)       ;
int ACL_Diag_RdCfgs (void)      ;
int ACL_Diag_RdIrqs (void)      ;

#define ACL_Addr    0x1D       // ACL I2C Address

#endif



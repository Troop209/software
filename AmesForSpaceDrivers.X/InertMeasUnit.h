/* 
 * File:   InertMeasUnit.h
 * Author: Norman McFarland
 *
 * Created on July 13, 2016, 2:46 PM
 */

#ifndef INERTMEASUNIT_H
#define	INERTMEASUNIT_H

#ifdef	__cplusplus
extern "C" {
#endif

    
int readACC(int  *a)    ;      //  6 byte buffer
int readMAG(int  *m)    ;      //  6 byte buffer
int readGYR(int *g)     ;      //  6 byte buffer
int enableIMU()        ;      //    no   buffer
int dumpIMU(char *ptr) ;      // 96 byte buffer
int gyro3_main(int argc, char *argv[])  ;
int AMG5_main(int argc, char *argv[])   ;

#define ACC_Addr    0x1D       // ACL I2C Address
#define MAG_Addr    0x1D       // MAG I2C Address
#define GYR_Addr    0X6B       // GYR I2C Address




#ifdef	__cplusplus
}
#endif

#endif	/* INERTMEASUNIT_H */


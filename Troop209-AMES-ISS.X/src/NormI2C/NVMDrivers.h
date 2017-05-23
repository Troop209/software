/* 
 * File:   NVMDrivers.h
 * Author: Norman McFarland
 *
 * Created on February 17, 2017, 1:30 PM
 */

#ifndef NVMDRIVERS_H
#define	NVMDRIVERS_H

#ifdef	__cplusplus
extern "C" {
#endif

int readNVM(char *Dptr, Byte BgnAddr, Byte Rlen)    ;
int writeNVM(char *Sptr, Byte BgnAddr, Byte Wlen)  ;
int testNVM(void)                           ;


#ifdef	__cplusplus
}
#endif

#endif	/* NVMDRIVERS_H */


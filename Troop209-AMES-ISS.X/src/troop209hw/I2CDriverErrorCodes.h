/* 
 * File:   I2CDriverErrorCodes.h
 * Author: Norman McFarland
 *
 * Created on September 13, 2016, 1:21 PM
 */

#ifndef I2CDRIVERERRORCODES_H
#define	I2CDRIVERERRORCODES_H

#ifdef	__cplusplus
extern "C" {
#endif

    // I2C2 driver Stage error codes (max range 0-F) (in I2C2_TalkToDEvice)
    const int ErrNull        = 0x0000 ;    // No error (so far)
    const int ErrStartBit    = 0x0010 ;
    const int ErrSndAdWt     = 0x0020 ;
    const int ErrSndStg      = 0x0030 ;
    const int ErrRestart     = 0x0040 ;
    const int ErrSndAdRd     = 0x0050 ;
    const int ErrGetStg      = 0x0060 ;
    const int ErrStopBit     = 0x0070 ;
    const int ErrPing        = 0x0080 ;
    #define ErrNotI2C        = 0x00F0 ;



#ifdef	__cplusplus
}
#endif

#endif	/* I2CDRIVERERRORCODES_H */


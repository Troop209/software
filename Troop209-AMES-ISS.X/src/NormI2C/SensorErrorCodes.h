/* 
 * File:   SensorErrorCodes.h
 * Author: Norman McFarland
 *
 * Created on September 13, 2016, 1:15 PM
 */

#ifndef SENSORERRORCODES_H
#define	SENSORERRORCODES_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    const int BufLenOverflow = 0X0006 ;  // 0x0006   (See SensaorDrivers.c)

// Stage or Misc error code values    

    const int StageNull     = 0x0000 ;    // No error (so far)
    const int Stage1        = 0x0100 ;
    const int Stage2        = 0x0200 ;
    const int Stage3        = 0x0300 ;
    const int Stage4        = 0x0400 ;
    const int Stage5        = 0x0500 ;
    const int Stage6        = 0x0600 ;
    const int Stage7        = 0x0700 ;
    const int Misc1         = 0x0800 ;
    const int Misc2         = 0x0900 ;
    const int Misc3         = 0x0A00 ;
    const int Misc4         = 0x0B00 ;
    const int Misc5         = 0x0C00 ;
    const int Misc6         = 0x0D00 ;
    const int Misc7         = 0x0E00 ;

// Driver Code Numbers (max range 0-F)
    const int DrvrNull      = 0x0000 ;    // No error (so far)
    const int DrvrExtRTC    = 0x1000 ;
    const int DrvrIntRTC    = 0x2000 ;
    const int DrvrRedGrnBlu = 0x3000 ;
    const int DrvrTemHumPrs = 0x4000 ;
    const int DrvrEncoder   = 0x5000 ;
    const int DrvrRadiation = 0x6000 ;
    const int DrvrMotor     = 0x7000 ;
    const int DrvrNotAvail  = 0xF000 ;



#ifdef	__cplusplus
}
#endif

#endif	/* SENSORERRORCODES_H */


/* 
 * File:   PIC24_I2C2_R5.h
 * Author: Norman McFarland
 *
 * Created on December 28, 2015, 12:12 PM
 */

#ifndef I2CDRIVERS_H
#define	I2CDRIVERS_H

// message level functions USER CALLS!
char    i2c2_ping(char addr)                             ; // user access
int     i2c2_TalkToDevice(char addr, char I2C2_slen, 
        char* I2C2_sbuf, char I2C2_rlen, char* I2C2_rbuf) ;

void    I2C2_Parameters(void)                            ;
// bit level functions (bit times)
void    i2c2_init(int KHz)                               ; // user access
int     i2c2_reset_bus(void)                             ;
int     i2c2_start(void)                                 ;
int     i2c2_restart(void)                               ;
int     i2c2_stop(void)                                  ;
// byte level functions (byte times))
int     i2c2_send_addr_rd (char addr)                     ;
int     i2c2_send_addr_wt (char addr)                     ;
int     i2c2_send_byte_ack(char data)                     ;
int     i2c2_read_byte_ack(int nack)                      ;
// string level functions
int     i2c2_send_string(char addr, char I2C2_slen, char *I2C2_sbufptr)  ;
int     i2c2_read_string(char addr, char I2C2_rlen, char *I2C2_rbufptr)  ;
int     i2c2_Diag_Report(void)      ;
int     i2c2_Diag_Clear(void)       ;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Define I2C2 parameters
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    #define I2C_GET_DIAG  32    // SPECIAL Diagnostic report address. Results at I2C2_Diagnostics[0]
    #define I2C_CLR_DIAG  35    // SPECIAL Diagnostic Clear  address

    // const char  I2C_GET_DIAG    =  32 ;         // Real Time Clock Address
    // const char  I2C_CLR_DIAG    =  35 ;         // Real Time Clock Address

    // I2C2 parameter Config
    // At 100,000 bits/sec and 16Mhz clock, value should get to about 160. 16 bit word fine.
    
    #define I2C2_ACK     0    ;   
    #define I2C2_NAK     1    ;
    
    #define I2C2_Read    0    ;    
    #define I2C2_Write   1    ;

    #define I2C2_KHz        75 ; // K bits per second (desired baud rate))

// Error COde Structure Make up
//
//  |  15  14  13  12  |  11  10   9   8  |   7   6   5   4  |   3   2   1   0 |
//  |                  |                  |                  |                 |
//  |  SENSOR Function | Driver Identifier| Tlk2Dev  Func Blk| I2C Error Code  |
//  |                  |                  |                  |                 |
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

// I2C Error COdes (max range 0-F)
    #define I2C2_SUCCESS    0 ;  // 0x0000
    #define I2C2_NAK        1 ;  // 0x0001
    #define I2C2_TIME_OUT2  2 ;  // 0x0002
    #define I2C2_TIME_OUT3  3 ;  // 0x0003
    #define I2C2_TIME_OUT4  4 ;  // 0x0004
    #define I2C2_TIME_OUT5  5 ;  // 0x0005
//  #define BufLenOverflow  6 ;  // 0x0006   (See SensaorDrivers.c)

//  const int ErrNull     = 0x0000 ;    // No error (so far)
//  const int ErrStartBit = 0x0010 ;
//  const int ErrSndAdWt  = 0x0020 ;
//  const int ErrSndStg   = 0x0030 ;
//  const int ErrRestart  = 0x0040 ;
//  const int ErrSndAdRd  = 0x0050 ;
//  const int ErrGetStg   = 0x0060 ;
//  const int ErrStopBit  = 0x0070 ;
//  const int ErrPing     = 0x00F0 ;


// Stage or Misc error code values    

//  const int StageNull     = 0x0000 ;    // No error (so far)
//  const int Stage1        = 0x0100 ;
//  const int Stage2        = 0x0200 ;
//  const int Stage3        = 0x0300 ;
//  const int Stage4        = 0x0400 ;
//  const int Stage5        = 0x0500 ;
//  const int Stage6        = 0x0600 ;
//  const int Stage7        = 0x0700 ;
//  const int Misc1         = 0x0800 ;
//  const int Misc2         = 0x0900 ;
//  const int Misc3         = 0x0A00 ;
//  const int Misc4         = 0x0B00 ;
//  const int Misc5         = 0x0C00 ;
//  const int Misc6         = 0x0D00 ;
//  const int Misc7         = 0x0E00 ;
//  const int Misc7         = 0x0E00 ;

// Driver Code Numbers (max range 0-F)
//  const int DrvrNull      = 0x0000 ;    // No error (so far)
//  const int DrvrExtRTC    = 0x1000 ;
//  const int DrvrIntRTC    = 0x2000 ;
//  const int DrvrRedGrnBlu = 0x3000 ;
//  const int DrvrTemHumPrs = 0x4000 ;
//  const int DrvrEncoder   = 0x5000 ;
//  const int DrvrRadiation = 0x6000 ;
//  const int DrvrMotor     = 0x7000 ;
//  const int DrvrNotAvail  = 0xF000 ;


//   NOT USED:  Sensor Function COdes (max range 0-7) (NO 8-F)
    // const int FncnInit     = 0x1000 ;
    // const int FncnRead     = 0x2000 ;
    // const int FncnSend     = 0x3000 ;
    // const int FncnFormat   = 0x4000 ;
    // const int FncnDiag     = 0x5000 ;
    // const int FncnNdr1     = 0x6000 ;
    // const int FncnNdr2     = 0x7000 ;
    // const int FncnXXX      = 0x8000 -> 0xFFFF ;     Niot available- NEGATIVE Values


#endif // I2CDRIVERS_H
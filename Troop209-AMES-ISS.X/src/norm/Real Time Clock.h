/* 
 * File:   Real Time Clock.h
 * Author: Norman McFarland
 *
 * Created on December 28, 2015, 11:54 AM
 */

//#ifndef REAL_TIME_CLOCK_H
//#define	REAL_TIME_CLOCK_H

//void Real_Time_Clock_Parameters ( void)
   // Real Time Clock info:
    //
    // BCD formated response means tens in upper nibble as 0-09 and units in lower nibble3 as 09-9
    // 
    // 0x00 BCD Seconds               0SSSssss
    // 0x01  BCD minutes              0MMMmmmm
    // 0x02  BCD hours  +12/24 flag   0xHHhhhh
    // 0x03  BCD Day of week          00000ddd
    // 0x04  BCD day                  00DDdddd
    // 0x05  BCD month                00MMmmmm
    // 0x06  BCD Year                 YYYYyyyy
    // 0x07  control byte value

    // Real Time Clock Variables
    #define  I2C2_ADDR_RTC =  0x68 ;         // Real Time Clock Address

    int     RTC_SET_LENGTH  =   9   ;   // for RTC memory Address
    int     RTC_GET_LENGTH  =   1   ;   // send 1 byte of address only
    int     RTC_RSP_LENGTH  =   8   ;   // Time Setting String
                              //ad Ss Mm  Hh DW Dd Mm Yy Ctl X X X X X X X 
    char    STG_SET_TIME[16] = {00,19,11,0x70,02,07,05,02,00,00,00,00,00,00,00,00} ;
    char    STG_GET_TIME[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;

    char    STG_I2C2_RESPONSE_BUFFER[64] = {0} ;
//
    char *PTR_SET_TIME              = STG_SET_TIME   ;              // Pointer to RTC set time string
    char *PTR_I2C2_RESPONSE_BUFFER  = STG_I2C2_RESPONSE_BUFFER ;    // Pointer to RTC return Time String
//
    // End RTC setup variables
 //    function 00

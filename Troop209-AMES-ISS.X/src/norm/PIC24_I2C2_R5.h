/* 
 * File:   PIC24_I2C2_R5.h
 * Author: Norman McFarland
 *
 * Created on December 28, 2015, 12:12 PM
 */

#ifndef PIC24_I2C2_R5_H
#define	PIC24_I2C2_R5_H


void    I2C2_Parameters(void)                            ;
// bit level functions (bit times)
void    i2c2_init(int KHz)                               ; // user access
int     i2c2_reset_bus(void)                             ;
int     i2c2_start(void)                                 ;
int     i2c2_restart(void)                               ;
int     i2c2_stop(void)                                  ;
// byte level functions (byte times))
int     i2c2_send_addr_rd (int addr)                     ;
int     i2c2_send_addr_wt (int addr)                     ;
int     i2c2_send_byte_ack(int data)                     ;
char    i2c2_read_byte_ack(int nack)                     ;
// string level functions
int     i2c2_send_string(int addr, int I2C2_slen, char *I2C2_sbufptr)  ;
int     i2c2_read_string(int addr, int I2C2_rlen, char *I2C2_rbufptr)  ;
// message level functions
char    i2c2_ping(char addr)                             ; // user access
int     i2c2_TalkToDevice(int addr, int I2C2_slen,  
        char *I2C2_sbufptr, int I2C2_rlen, char *I2C2_rbufptr) ; // user access

int     i2c2_Diag_Report(void)      ;
int     i2c2_Diag_Clear(void)       ;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Define I2C2 parameters
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    //int i2c_timeout = 5200    ;
    #define I2C2_Bit_TimeOut  = 5200   
    #define I2C2_Byte_TimeOut = 52000  

    // I2C2 parameter Config
    // At 100,000 bits/sec and 16Mhz clock, value should get to about 160. 16 bit word fine.
    
    #define I2C2_ACK     0    ;   
    #define I2C2_NAK     1    ;
    
    #define I2C2_Read    0    ;    
    #define I2C2_Write   1    ;

    // enum    I2C2_Stat  {I2C2_SUCCESS, I2C2_NAK, I2C2_TIME_OUT, I2C2_TIME_OUT2, I2C2_TIME_OUT3 }   ;
    #define I2C2_SUCCESS    0 ;
    #define I2C2_NAK        1 ;
    #define I2C2_TIME_OUT   2 ;
    #define I2C2_TIME_OUT2  3 ;
    #define I2C2_TIME_OUT3  4 ;

    #define I2C2_KHz        75 ; // K bits per second (desired baud rate))

    #define I2C2_DIAG_ADDR  32 ;   // SPECIAL Diagnostic address

#endif	/* PIC24_I2C2_R5_H */


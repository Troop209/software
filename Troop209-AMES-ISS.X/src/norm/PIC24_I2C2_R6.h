/* 
 * File:   PIC24_I2C2_R5.h
 * Author: Norman McFarland
 *
 * Created on December 28, 2015, 12:12 PM
 */

#ifndef PIC24_I2C2_R6_H
#define	PIC24_I2C2_R6_H

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

    // enum    I2C2_Stat  {I2C2_SUCCESS, I2C2_NAK, I2C2_TIME_OUT, I2C2_TIME_OUT2, I2C2_TIME_OUT3 }   ;
    #define I2C2_SUCCESS    0 ;
    #define I2C2_NAK        1 ;
    #define I2C2_TIME_OUT   2 ;
    #define I2C2_TIME_OUT2  3 ;
    #define I2C2_TIME_OUT3  4 ;

    #define I2C2_KHz        75 ; // K bits per second (desired baud rate))


/*  I2C2_TalkToDevice Response Error COde Format
 *  FOrmat:
 *  76543210
 *  ffffssss    where f= fault & s =section
 *      0000    SUCCESS
 *      0001    I2C2 Start bit
 *      0010    I2C2 Send Address-Write (often occurs if no response from slave).
 *      0011    I2C2 Send String
 *      0100    I2C2 Restart
 *      0101    I2C2 Send ADrress-Read
 *      0110    I2C2 Read STring
 *      0111    I2C2 Stop bit
 *      1000     (Not Defined Yet) 
 *      1001     (Not Defined Yet)
 *      1010     (Not Defined Yet)
 *      1011     (Not Defined Yet)
 *      1100     (Not Defined Yet)
 *      1101     (Not Defined Yet)
 *      1110     (Not Defined Yet)
 *      1111    I2C2 Poll Error
 * 
 *  0000        SUCCESS
 *  0001        Negative Success (fail)
 *  0010        TimeOut #1
 *  0011        TimeOut #2
 *  0100        TimeOut #3
 *  0101         (Not Defined Yet)
 *  0110         (Not Defined Yet)
 *  0111         (Not Defined Yet)
 *  1000         (Not Defined Yet)
 *  1001         (Not Defined Yet)
 *  1010         (Not Defined Yet)
 *  1011         (Not Defined Yet)
 *  1100         (Not Defined Yet)
 *  1101         (Not Defined Yet)
 *  1110         (Not Defined Yet)
 *  1111         (Not Defined Yet)
 *  */
/*
// Diagnostic STring Format                         Count of:
    I2C2_Diagnostics[0] =  I2C2_Msg_Ack_cnt         Number of ACknowledged Messages   
    I2C2_Diagnostics[1] =  I2C2_Msg_Nak_cnt         Number of Negative ACknowledged Messages
    I2C2_Diagnostics[2] =  I2C2_Msg_Reset_cnt       Number of times I2c bus reset (due to transmission errors
    I2C2_Diagnostics[3] =  I2C2_Msg_Req_cnt         Number of Requested messages by user
    
    I2C2_Diagnostics[4] =  I2C2_Snd_Byte_cnt_TO0  
    I2C2_Diagnostics[5] =  I2C2_Snd_Byte_cnt_TO1  
    I2C2_Diagnostics[6] =  I2C2_Snd_Byte_cnt_TO2  
    I2C2_Diagnostics[7] =  I2C2_Snd_Byte_cnt_TO3  
    I2C2_Diagnostics[8] =  I2C2_Snd_Byte_cnt_ACK  
    I2C2_Diagnostics[9] =  I2C2_Snd_Byte_cnt_NAK  

    I2C2_Diagnostics[10] =  I2C2_Get_Byte_cnt_TO0  
    I2C2_Diagnostics[11] =  I2C2_Get_Byte_cnt_TO1  
    I2C2_Diagnostics[12] =  I2C2_Get_Byte_cnt_TO2  
    I2C2_Diagnostics[13] =  I2C2_Get_Byte_cnt_ACK  
    I2C2_Diagnostics[14] =  I2C2_Get_Byte_cnt_NAK  
    I2C2_Diagnostics[15] =  0       ;   // not used
 */
#endif	/* PIC24_I2C2_R6_H */


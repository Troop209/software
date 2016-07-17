/* 
 * File:   TempPresHumd.h
 * Author: Norman McFarland
 *
 * Created on April 24, 2016, 3:14 PM
 */

#ifndef TEMPPRESHUMD_H
#define	TEMPPRESHUMD_H


#define THP_Addr     0X39 

int     THP_Configure1 (void)       ;
int     THP_Configure2 (void)       ;   
int     THP_Configure3 (void)       ;
int     THP_Read_Values (void)      ;
void    THP_LoadConfigs (void)      ;
signed   long THP_compensate_T_int32(signed long adc_T) ;
unsigned long THP_compensate_P_int64(signed long adc_P) ;
unsigned long THP_compensate_H_int32(signed long adc_H) ;
void    THP_LoadRawValues (void)    ;
void    THP_SetBuffer(char* Lcl_ReadBuf, int SetLen, char Fill) ;
void    THP_SwapBytes (char* Lcl_ReadBuf, int Pairs)    ;

#endif	/* TEMPPRESHUMD_H */

           
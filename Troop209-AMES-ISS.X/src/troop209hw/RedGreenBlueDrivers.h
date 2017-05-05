/* 
 * File:   RedGreenBlue.h
 * Author: Norman McFarland
 *
 * Created on April 18, 2016, 5:13 PM
 */

#ifndef REDGREENBLUE_H
#define	REDGREENBLUE_H


int initRGB (void)       ;
int RGB_Configure2 (void)       ;
int RGB_ClearIrqs (void)        ;
int RGB_ReadStatus (void)       ;
int readRGB(void)       ;

#define RGB_Addr    0X39         // RGB I2C Address

#endif

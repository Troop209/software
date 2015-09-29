/* 
 * Please add your name as you test and run your environment
 * Ken Yu 09/15 v1
 * Joe Pluta 09/28 v2
 */
#include <xc.h>

#pragma config FWDTEN = OFF, JTAGEN = OFF

void delay(void);

unsigned int ctr = 0;
unsigned int delayVal = 2048;

int main(void)
{
    LATA = 0;
    TRISA = 0xFF00;

    while(1)
    {
        LATA = 0x0055;
        delay();

        LATA = 0x00AA;
        delay();

        ctr++;
    }
}

void delay(void)
{
    unsigned int i,j;

    for (i = 0; i < delayVal; i++)
    {
        for (j = 0; j < 20; j++);
    }
}



/**********************************************************************
* © 2013 Microchip Technology Inc.
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (Microchip) licenses this software to you
* solely for use with Microchip dsPIC digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED AS IS.  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
**********************************************************************/


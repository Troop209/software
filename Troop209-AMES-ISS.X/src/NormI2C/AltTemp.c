// Alt TYemp sensor file
#include "system.h"
#include "../core/resistiveSensors.h"

int aTmpCount    = 0 ;
int aTmpLookup   = 0 ;
int aTmpPerCent  = 0 ;
int aTmpLowTemp  = 0 ;
int aTmpHigthTemp= 0 ;
int aTmpDeltaTemp= 0 ;
int altTemp     = 0 ;

int TempTable[33]={300,261,213,187,168,155,143,134,125,118,111,104, 98, 92, 87, 82,
                    76, 71, 66, 61, 56, 51, 46, 40, 34, 28, 22, 14,  6, -4,-18,-38,-50} ;

int readAltTemp(void)
{   aTmpLookup   = 0 ;
    aTmpCount    = resistiveSensors.readQ1()  ;   // function from NESI  resistiveSensors
    aTmpLookup   = (aTmpCount/32)   ;
    aTmpPerCent  = aTmpCount % 32 ;
    aTmpLowTemp  = TempTable[aTmpLookup];
    aTmpHigthTemp= TempTable[aTmpLookup+1];
    aTmpDeltaTemp= aTmpHigthTemp-aTmpLowTemp;
    altTemp     = aTmpHigthTemp - ((aTmpDeltaTemp * aTmpPerCent) /32)    ; 
    return (altTemp)    ;
}


int aLhtCount    = 0 ;
int aLhtLookup   = 0 ;
int aLhtPerCent  = 0 ;
int aLhtLowTemp  = 0 ;
int aLhtHigthTemp= 0 ;
int aLhtDeltaTemp= 0 ;
int altLight     = 0 ;

int LightTable[33]={300,261,213,187,168,155,143,134,125,118,111,104, 98, 92, 87, 82,
                    76, 71, 66, 61, 56, 51, 46, 40, 34, 28, 22, 14,  6, -4,-18,-38,-50} ;

int readAltLight(void)
{   aLhtLookup   = 0 ;
    aLhtCount    = resistiveSensors.readQ1()  ;   // function from NESI  resistiveSensors
    aLhtLookup   = (aLhtCount/32)   ;
    aLhtPerCent  = aLhtCount % 32 ;
    aLhtLowTemp  = LightTable[aLhtLookup];
    aLhtHigthTemp= LightTable[aLhtLookup+1];
    aLhtDeltaTemp= aLhtHigthTemp-aLhtLowTemp;
    altLight     = aLhtHigthTemp - ((aLhtDeltaTemp * aLhtPerCent) /32)    ; 
    return (altLight)    ;
}

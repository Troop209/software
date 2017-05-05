#include "../core/system.h"

/* Write something to RB11 */
static void RB11out(unsigned short value)
{
    _RB11 = value;
    PORTBbits.RB11 = value;
    
    _RB11 = 0;
    _RB11 = 1;
    
    PORTBbits.RB11 = 0;
    PORTBbits.RB11 = 1;
    
} 


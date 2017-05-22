
#include "system.h"
#include "nesi.h"

int t5prev  = 0 ;
int t5curr  = 0 ;

unsigned int CWbit      = 0 ;
unsigned int CCWbit     = 0 ;
unsigned int CWfilter   = 0 ;
unsigned int CCWfilter  = 0 ;
unsigned int CWcount    = 0 ;
unsigned int CCWcount   = 0 ;
unsigned int CWprev     = 0 ;
unsigned int CCWprev    = 0 ;

unsigned int LoadCount  = 0 ;
unsigned int optoZero   = 0 ;
unsigned int QuadWidth  = 0 ;                
unsigned int quadCount  = 0 ;





unsigned long CWhistory        = 0 ;
unsigned long CCWhistory       = 0 ;
unsigned long Quadhistory      = 0 ;

int readOptos(void)
{   t5prev  = t5curr    ;   // Save current to9 previous
    t5curr  = TMR5 ;
    if( t5prev  == t5curr)
    { // not time to read bits again
        return (-1)  ;
    }
    // IS time to read bits
    // Get bit values
    CWbit   = _RE5  ;
    CCWbit  = _RE6 ;
    
    CWbit   &= 0x0001   ; // get rid all but LSB
    CCWbit  &= 0x0001   ; // get rid all but LSB
    
    // SHift bit values into history word
    CWhistory  = CWhistory  < 1 ;
    CCWhistory = CCWhistory < 1 ; 
    CWhistory  = CWhistory  | CWbit ;
    CCWhistory = CCWhistory | CWbit ;
    
    // Check for Same/Different bit values
    if (CWbit != CWprev)
    {   CWfilter = 0;
    }
    else
    {   CWfilter++  ;
        if (CWfilter >=8)
        {   CWcount = LoadCount-8  ;
            CWfilter = 0 ; 
        }
        if (CCWbit != CCWprev)
        {   CCWfilter = 0;
        }
        else
        {   CCWfilter++  ;
            if (CCWfilter >=8)
            {   CCWcount = LoadCount-8  ;
            CCWfilter = 0 ; 
            }
            CWcount--  ;
            CCWcount-- ;
        }
    
        // Have we reached center of current bit?
        if ( (CWcount == 0) || (CCWcount == 0) )
        {   // YES! time to build "quad" (00, 01, 10, 11)
            // Reset xxcount widths
            Quadhistory = Quadhistory < 1 ;
            Quadhistory = Quadhistory | CCWbit  ;
            Quadhistory = Quadhistory < 1 ;
            Quadhistory = Quadhistory | CWbit  ;
            // Was this first pattern bit?
            optoZero = Quadhistory & 0X000000FF ;
            if (optoZero == 0)
            {   // go 1/2 quad width to sample in middle of bit
                LoadCount = QuadWidth > 2 ;
                quadCount = 0   ; // still at zero, (no 01,10, or 11 quads in last 4 quad times)
            }
            else
            {   // go full bit width before next sampling (middle next bit)
                CWcount = QuadWidth   ;
                CCWcount= QuadWidth   ;                
                quadCount++           ;
            }
        // Think we ready to check out pattern

        }
        
    }
    return (0)  ;
}
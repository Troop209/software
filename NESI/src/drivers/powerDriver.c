#include "system.h"
#include "powerDriver.h"


/******************************************************
 *
 * Power Driver A
 *
 ******************************************************/

static void setOutputA(boolean desiredOutputState)
{
    _TRISE7 = 0;    /* configure port as output */
    _RE7 = desiredOutputState; /* set the output */

}static void setOnA(void)
{
    setOutputA(1); /* set the output to be open */
}

static void setOffA(void)
{
    setOutputA(0); /* set the output to be closed */
}

static void initializeA(void)
{
    /* no itialization needed*/
}

static boolean readInputA(void)
{
    _TRISG6 = 1; /* configure port as input */
    return _RG6; /* return the value on the pin */
}

static boolean failureA(void)
{
    boolean error = 0;      /* Initialize variable error  to 0 */
    powerDriverA.on();      /* Turns powerDriverA on */
    if( powerDriverA.status()== 1 )
    {                    /* Error if powerDriverA  is on and status = 1 */
        error = 1;       /** Note: something must be pulgged into JP2 **/
    }
    powerDriverA.off();     /*Turns powerDriverA off*/
    
    if(powerDriverA.status()==0)
    {                               /* Error if powerDriverA is off and status = 0 */
        error = 1;                  /** Note: something must be pulgged into JP2 **/
    }
    
    return error;       /* If error = 0 then nothing is wrong. If error = 1 then something is wrong*/
}



/******************************************************
 *
 * Power Driver B
 *
 ******************************************************/


static void setOutputB(boolean desiredOutputState)
{
    _TRISE0 = 0;    /* configure port as output */
    _RE0 = desiredOutputState; /* set the output */

}static void setOnB(void)
{
    setOutputB(1); /* set the output to be open */
}

static void setOffB(void)
{
    setOutputB(0); /* set the output to be closed */
}

static void initializeB(void)
{
   /* no Initialization needed*/
}

/*
   Warning:
      Not an actual test of InputB
 */
static boolean readInputB(void)
{
    return 1; /* Assumption that powerDriverB is working */
}

static boolean failureB (void)
{
    boolean error = 0;      /* No way of testing error so set error = 0 */
    return error;       /* Note: Assumption that powerDriverB is working */
}





const PowerDriver powerDriverA = {set:setOutputA, isFailure:failureA, status:readInputA, init:initializeA, on:setOnA, off:setOffA},
                  powerDriverB = {set:setOutputB, isFailure:failureB, status:readInputB, init:initializeB, on:setOnB, off:setOffB};

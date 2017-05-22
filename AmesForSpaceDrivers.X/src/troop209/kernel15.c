#include <nesi.h>
#include <led.h>
#include "SD_support.h"

extern SDConfig config;

int kernel15(void)
{
    SDConfigFile.get(&config);
    
    return 0;
}


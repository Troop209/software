#include <nesi.h>

/*
 * USB Connect to update configuration
 */
int kernel13(void)
{
    nesi.init();
    usb.connect();
    while(1){} //infinite loop
    
}


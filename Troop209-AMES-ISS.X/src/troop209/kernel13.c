#include <nesi.h>

/*
 * USB Connect to update configuration
 */
int kernel13(void)
{
    nesi.init();
    usb.connect();
    delay(100000);
    
    return 0;
}


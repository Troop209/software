#include <nesi.h>

int main(void) {
    nesi.init();
    
 //   camera.on();
    camera.getPix("aaa.tiff");
  //  camera.off();
    
    return (0);
}


int main2(void)
{
    // initialize all modules
    nesi.init();

    usb.connect();

    char input[64] = {0};
    int bytesRead = 0;
    Boolean debugEnable = YES;
    usb.setDebug(ON);

    while(1)
    {
        if((bytesRead = usb.read(input,64)))
        {
            input[bytesRead] = '\0'; // terminate string
            usb.debugPrint("At time %s, %d bytes of data received:\r\n\"%s\"\r\n\r\n",
                           dateTime.getStamp(), bytesRead, input);

            if(debugEnable == YES)
            {
                debugEnable = NO;
                usb.setDebug(OFF);
                while(!usb.print("Disabling Debug Print. The next input will not be printed.\r\n")); // keep trying until successful
            }
            else
            {
                debugEnable = YES;
                usb.setDebug(ON);
                usb.print("Enabling Debug Print.\r\n"); // don't retry print if unsuccessful
            }
        }
    }

    return 0;
}

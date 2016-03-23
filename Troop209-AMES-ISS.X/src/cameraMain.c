#include "drivers/system.h"
#include "drivers/camera.h"
#include "drivers/datalog.h"

int cameraMain(void)  
{
    system.init();
    
    camera.on();
    camera.getPic();
    camera.off();
    
    return (0);
}

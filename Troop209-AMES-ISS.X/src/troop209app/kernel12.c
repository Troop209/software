//#include "../drivers/system.h"
//#include "../drivers/moisture.h"
//#include "../drivers/led.h"

#include <nesi.h>
#include <led.h>
#include <moisture.h>
#define STEP_WAIT 3000


int kernel12(void)
{
    // initialize all modules
    system.init();
    nesi.init();
    moisture.init();
    CopCarInit();
    
    //  Never ending loop
    //    1. Initialize all hardware
    //    2. Perform any application setup
    //    3. Run scheduled tasks
    //    4. Wait for an external event to loop
    
    while (1) {
        
        init();
    
        setup();
    
        schedule();
        
        waitForExternalEvent();
    
    }
    
    
    return 0;
}

// init - initialize all hardware
// (stub)
void init() {
    
    flash(1);
    wait(1000);

}

// setup - perform application setup
// (stub)
void setup() {
    
    flash(2);
    wait(1000);
            
}

// schedule - run scheduled tasks
// Loop (in this case, while dip switches are off)
//   1. Zero the barrel
//   2. Log data measurements
//   3. Take pictures
void schedule() {
    
    zeroBarrel();
    
    readSensors();
    
    //   1. Position to plate
    //   2. Take picture camera1
    //   3. Take picture camera2
    int plate = 0;
    for (plate = 1; plate <= 3; plate++) {
        
        positionToPlate(plate);

        takePictureCamera1();

        takePictureCamera2(); 
        
    }
    
}


// zeroBarrel - Return the barrel to zero position
// (stub)
void zeroBarrel() {
    
    flash(3);
            
}

// readSensors - Read sensors and log to SD card
// (stub)
void readSensors() {
    
    flash(4);
    wait(STEP_WAIT);
            
}

// positionToPlate - Turn the barrel to the specified plate
// (stub)
void positionToPlate(int plate) {
    
    flash(5);
    flash(plate);
    wait(STEP_WAIT);
            
}

// takePictureCamera1 - Take picture on camera 1
// (stub)
void  takePictureCamera1() {
    
    flash(6);
    wait(STEP_WAIT);
     // Set military time and Date
    DateAndTime timeTemp;

    // time = 12:52:50
    timeTemp.hour = 16;
    timeTemp.minute = 0;
    timeTemp.second = 0;

    // date = 10/29/13
    timeTemp.month = NOVEMBER;
    timeTemp.day = 25;
    timeTemp.year = 13;

    dateTime.set(timeTemp);

    //   camera.on();
    char filename[32] = {0};
    sprintf(filename, "camera_1_%s.jpg", dateTime.getStamp());

    // the ':' character is an invalid character, so it needs to be changed
    String temp = filename;
    while (*temp) {
        if (*temp == ':')
            *temp = '.';
        ++temp;
    }
    
    // first character is 1 for file name
    Boolean b = camera.getPix(filename);
}

// takePictureCamera2 - Take picture on camera 2
// (stub)
void takePictureCamera2() {
    
    flash(7);
    wait(STEP_WAIT);
    
         // Set military time and Date
    DateAndTime timeTemp;

    // time = 12:52:50
    timeTemp.hour = 16;
    timeTemp.minute = 0;
    timeTemp.second = 0;

    // date = 10/29/13
    timeTemp.month = NOVEMBER;
    timeTemp.day = 25;
    timeTemp.year = 13;

    dateTime.set(timeTemp);

    //   camera.on();
    char filename[32] = {0};
    sprintf(filename, "camera_2_%s.jpg", dateTime.getStamp());

    // the ':' character is an invalid character, so it needs to be changed
    String temp = filename;
    while (*temp) {
        if (*temp == ':')
            *temp = '.';
        ++temp;
    }
    
    // first character is 1 for file name
    Boolean b = camera2.getPix(filename);
}

// waitForExternalEvent - Wait for a DIP switch to be set
// As long as DIPs are all zero, sit in the loop
void waitForExternalEvent() {
    
    while ( getDips() == 0 );
}

int getDips() {
    int q1, dips;
    
      q1 = moisture.readQ1();
      
      if (q1 > 345)
          dips = 0b000;
      else if (q1 > 310)
          dips = 0b001;
      else if (q1 > 270)
          dips = 0b010;
      else if (q1 > 216)
          dips = 0b011;
      else if (q1 > 156)
          dips = 0b100;
      else if (q1 > 99)
          dips = 0b101;
      else if (q1 > 33)
          dips = 0b110;
      else 
          dips = 0b111;
      
      return dips;
}

void flash(int flashes) {
    ledB.dutycycle(0);
    ledR.dutycycle(100);
    delay(250);
    ledR.dutycycle(0);
    while (flashes--) {
        delay(250);
        ledB.dutycycle(100);
        delay(250);
        ledB.dutycycle(0);
    };
}

void CopCarInit() {
    ledR.init();
    ledB.init();
}



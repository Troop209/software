/* 
 * File:   Switch_Align_Home.h
 * Author: Norman McFarland
 *
 * Created on February 27, 2016, 9:26 AM
 */

#ifndef SWITCH_ALIGN_HOME_H
#define	SWITCH_ALIGN_HOME_H
/* Read the ALIGN switch and see if carousel is aligned
 * for taking a picture with the camera
 * 
 * ALIGN Switch actions:
 *  switch open:    carousel is NOT aligned- do not take picture
 *  switch closed:  carousel IS aligned- take picture
 * Switch is ACtive Low (inverted)
 *  function return 0:  switch closed:  take picture
 *  function return 1:  switch open:    do not take picture
 * 
 * Function is self-initializing meaning
 *  it will stomp on any other configurations and re configure for switch operation
* 
 * Switch enters PIC 24 on pin 27 Port B bit 12
 * Switch is on Schematic trace EC5
 * Switch enters NESI board at JP8- pin 7
 * Switch uses no internal peripheral functions
 * Switchy is reaqd directly from port 12 pin
 */
int Read_Switch_Align(void)                      ;

/* Read the HOME switch and see if carousel is in position for travel
 * or if carousel needs to be moved into position for travel
 * 
 * HOME Switch actions:
 *  switch open:    carousel is NOT home- do not ready for travel
 *  switch closed:  carousel IS home- Ready for travel
 * Switch is ACtive Low (inverted)
 *  function return 0:  switch closed:  ready for travel
 *  function return 1:  switch open:    ready for travel- 
 * 
 * Function is self-initializing meaning
 *  it will stomp on any other configurations and re configure for switch operation
 * 
 * Switch enters PIC 24 on pin 54 Port D bit 6
 * Switch is on Schematic trace EC8
 * Switch enters NESI board at JP8- pin 10
 * 
 * Internal pulls are used for switch input
 */
int Read_Switch_Home(void)                       ;

/* The following 5 function groups allow user to control
 * the specified Power Digital Output and the respective NESI Device
 * setOutputXxxx allows user to directly command the device on setOutputXxxx(1)
 *   or to command the device off setOutputXxxx(0)
 * setOnXxxx turns device on
 * SetOffXxxx turns device off
 * initializeXxxxx initializes that port
 * 
 * initialize must be done as part of power up. 
 * On off commands will not work if poort not intialized.
 */
 
/* Turn CAMERA 1 On or OFF
 * Camera 1 control exits PIC 24 on pin 61 Port E bit 1
 * Camera 1 control signal is on Schematic trace CONTREOL_CAMERA
 * Camera 1 exits NESI board at JP15- pin 1 
 */
void setOutputCam1(Boolean desiredOutputState)   ;
void setOnCam1(void)                             ;
void setOffCam1(void)                            ;
void initializeCam1(void)                        ;

/* Turn CAMERA 2 On or OFF
 * Camera 2 control exits PIC 24 on pin 60 Port E bit 0
 * Camera 2 control signal is on Schematic trace CONTREOL_B
 * Camera 2 exits NESI board at JP11- pin 2 
 */
void setOutputCam2(Boolean desiredOutputState)   ;
void setOnCam2(void)                             ;
void setOffCam2(void)                            ;
void initializeCam2(void)                        ;

/* Turn LED 1 On or OFF
 * LED 1 control exits PIC 24 on pin 22 Port B bit 9
 * LED 1 control signal is on Schematic trace CONTREOL_LED_RED
 * LED 1 exits NESI board at RED LED CATHODE (-) pin 
 */
void setOutputLED1(Boolean desiredOutputState)   ;
void setOnLED1(void)                             ;
void setOffLED1(void)                            ;
void initializeLED1(void)                        ;

/* Turn LED 2 On or OFF
 * LED 2 control exits PIC 24 on pin 21 Port B bit 8
 * LED 2 control signal is on Schematic trace CONTREOL_LED_BLUE
 * LED 2 exits NESI board at BLUE LED CATHODE (-) pin 
 */
void setOutputLED2(Boolean desiredOutputState)   ;
void setOnLED2(void)                             ;
void setOffLED2(void)                            ;
void initializeLED2(void)                        ;

/* Turn Servo POWER On or OFF   (Enables/disables moves)
 * Servo control exits PIC 24 on pin 33 Port F bit 3
 * Servo control signal is on Schematic trace CONTROL_A
 * Servo exits NESI board at JP-2 pin 2
 * 
 * Servo movement is controlled by servo PWM output
 */
void setOutputServo(Boolean desiredOutputState)  ;
void setOnServo(void)                            ;
void setOffServo(void)                           ;
void initializeServo(void)                       ;


#endif	/* SWITCH_ALIGN_HOME_H */


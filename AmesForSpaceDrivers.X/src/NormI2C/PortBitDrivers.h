/* 
 * File:   PortBitDrivers.h
 * Author: Norman McFarland
 *
 * Created on November 20, 2016, 1:34 PM
 * 
 * Changes to file:
 * Rev 1 set Dec 2016
 * 1.1  Add port on off routines for 6Volt Enable
 * 
 * Rev 2 Jan 19 2017
 * 2.1. Add port on off routines for Energy Harvest Enable Disable (HarvEn (On) (Off)
 * 2.2. Add port on off routines for Servo Motor Enable NotorEn) (On) (Off)
 * 
 * Rev 3    Feb 09,2017
 * 3.1 Change un-needed HarvEn back to Diag (pulse for tracking function run times)
 * 
 * Rev 4 Mar 11, 2017
 * Add RG9 On/Off I/O bit functions
 */

#ifndef PORTBITDRIVERS_H
#define	PORTBITDRIVERS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif




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

// Motor control bits/pins                      ;
void setOutputMotCW(Boolean desiredOutputState) ;
void setOnMotCW(void)                           ;
void setOffMotCW(void)                          ;
void initializeMotCW(void)                      ;

void setOutputMotCCW(Boolean desiredOutputState);
void setOnMotCCW(void)                          ;
void setOffMotCCW(void)                         ;
void initializeMotCCW(void)                     ;

void setOutputRG9(Boolean desiredOutputState);
void setOnRG9(void)                          ;
void setOffRG9(void)                         ;
void initializeRG9(void)                     ;

void setOutput6Volt(Boolean desiredOutputState);
void setOn6Volt(void)                          ;
void setOff6Volt(void)                         ;
void initialize6Volt(void)                     ;

void setOutputDiag(Boolean desiredOutputState);
void setOnDiag(void)                          ;
void setOffDiag(void)                         ;
void initializeDiag(void)                     ;



void setOutputMotorEn(Boolean desiredOutputState);
void setOnMotorEn(void)                          ;
void setOffMotorEn(void)                         ;
void initializeMotorEn(void)                     ;

// DIAGNOSTICS for UARET 1 & 2 PINS
void setUart1Tx(Boolean desiredOutputState)   ;
void setUart1Rx(Boolean desiredOutputState)   ;
void setUart2Tx(Boolean desiredOutputState)   ;
void setUart2Rx(Boolean desiredOutputState)   ;

#endif	/* PORTBITDRIVERS_H */


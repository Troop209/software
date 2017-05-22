/**
 * NESI C329-UART camera2 Driver License
 * Copyright (C) 2013 Mickie Byrd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Change Log
 *
 * 11/17/2013 - Mickie Byrd
 *   Refactored camera2ComPort to Uart module.
 * 12/27/2013
 *   Changed error code that results if retrievePic2() cannot open file.
 *   Added wait after camera2 is powered down.
 *   Changed getPicture2() to return integer error code indicating point of
 *       failure.
 *   Refactored imageFile2 from FileStream to FSFILE*.
 *   Increased syncLimit to increase success rate.
 *   Increased power up wait time so that camera2 is fully ready.
 */

#include "camera.h"
#include "file.h"
#include "uart2.h"

/**
 * camera2ComPort Information
 *
 * camera2ComPort is the software wrapper for the UART module used by the camera2
 * module. uart1 will be used to provide serial I/O for the C329 camera2.
 */
static Uart camera2ComPort = {0};

/**
 * camera2Packet Information
 *
 * The C329-UART camera2 communicates using the following command set:
 *
 *   Command   | Cmd Token | Parameter 1   | Parameter 2   | Parameter 3   | Parameter 4
 *  -----------+-----------+---------------+---------------+---------------+--------------
 *   INITIAL   | FFFFFF01h | Baud Rate     | Color Type    | Preview Res.  | Compres. Res
 *   GET PIC   | FFFFFF04h | Pic. Type     | 00h           | 00h           | 00h
 *   SNAPSHOT  | FFFFFF05h | Snapshot Type | 00h           | 00h           | 00h
 *   RESET     | FFFFFF08h | Reset Type    | 00h           | 00h           | 00/FFh
 *   POWER OFF | FFFFFF09h | 00h           | 00h           | 00h           | 00h
 *   DATA      | FFFFFF0Ah | Data Type     | Length Byte 0 | Length Byte 1 | Length Byte 2
 *   SYNC      | FFFFFF0Dh | 00h           | 00h           | 00h           | 00h
 *   ACK       | FFFFFF0Eh | Cmd Token     | ACK counter   | 00h           | 00h
 *   NAK       | FFFFFF0Fh | 00h           | NAK counter   | Err. Number   | 00h
 *   QUALITY   | FFFFFF10h | Quality Level | 00h           | 00h           | 00h
 *
 * Each command is 8 bytes long. It starts with a 4 byte command token and up to four
 * parameters.
 */
typedef union {
  Uint8 raw[8];
  struct {
    Uint8 :8;
    Uint8 :8;
    Uint8 :8;
    Uint8 cmd2Token;
    Uint8 parameter1;
    Uint8 parameter2;
    Uint8 parameter3;
    Uint8 parameter4;
  };
} Camera2Packet;
/**
 * Cmd2Token Information
 *
 * The Cmd Token tells the camera2 which action to execute. The first 3 bytes of each Cmd Token
 * are all 0xFF, so they can be kept constant. The fourth byte corresponds to the action to
 * perform.
 */
typedef enum Cmd2Token {
    INITIAL   = 0x01,
    GET_PIC   = 0x04,
    SNAPSHOT  = 0x05,
    RESET     = 0x08,
    POWER_OFF = 0x09,
    DATA      = 0x0A,
    SYNC      = 0x0D,
    ACK       = 0x0E,
    NAK       = 0x0F,
    QUALITY   = 0x10
} Cmd2Token;


static Camera2Packet newCamera2Packet(void)
{
    static const Camera2Packet emptyPacket = {{0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00}};
    return emptyPacket;
}

static void sendPacket(Camera2Packet packetToSend)
{
    camera2ComPort.send(packetToSend.raw,8);
}

static Camera2Packet getPacket(void)
{
    T2CON = 0x0020;                 // disabled, prescaled, internal oscillator
    TMR2 = 0;                       // clear the timer
    PR2 = CLOCKS_PER_SEC/64/10 - 1; // interrupt at 1kHz (1ms)
    _T2IE = ON;                     // enable Timer2 to interrupt
    _T2IF = OFF;                    // clear the interrupt flag
    _T2IP = 4;                      // set priority to real-time
    T2CONbits.TON = 1;

    Camera2Packet inPacket = newCamera2Packet();
    while(camera2ComPort.size() < 8) if(!T2CONbits.TON) break;
    camera2ComPort.receive(inPacket.raw,8);
    return inPacket;
}

//static FileStream imageFile;
FSFILE* imageFile2 = NullPtr;

static int tempSize = 0;
static Int32 bytesToGet = 0;
static Int32 picsize = 0;
static Camera2Packet toCam, fromCam;

static CameraError getPicture2(void)
{
    toCam = newCamera2Packet(),
    fromCam = newCamera2Packet();
    toCam.cmd2Token = SYNC;
    int syncTries = 0;

    //if(!imageFile.open)
    if(!imageFile2)
        return NO_FILE;

    /* try and get sync with camera */
    while(!(fromCam.cmd2Token == ACK && fromCam.parameter1 == SYNC)) // until ACK received
    {
        if(syncTries == 3)
            camera2ComPort.baudrate(115200);
        camera2ComPort.flush();
        sendPacket(toCam); // send SYNC command
        pause(25);
        fromCam = getPacket(); // get reply

        if(syncTries < 7)
            ++syncTries;
        else
            return NO_HANDSHAKE;
    }

    /* acknowledge camera's sync request */
    toCam = newCamera2Packet(); // initialize packet
    toCam.cmd2Token = ACK;      // acknowledge
    toCam.parameter1 = SYNC;   // the sync

    syncTries = 0;
    while(!(fromCam.cmd2Token == SYNC)) // wait for camera SYNC request
    {
        fromCam = getPacket();

        if(syncTries < 7)
            ++syncTries;
        else
            return NO_SYNC;
    }

    sendPacket(toCam);

    /* configure camera */
    toCam = newCamera2Packet(); // initialize packet
    toCam.cmd2Token = INITIAL;  // initial configuration
    toCam.parameter1 = 0x04;   // 115,200 baud rate
    toCam.parameter2 = 0x87;   // compress color
    toCam.parameter3 = 0x01;   // 80x60 preview resolution
    toCam.parameter4 = 0x07;   // 640x480 compression resolution
    sendPacket(toCam);

    /* if camera acknowledges changes, change the UART baud rate */
    fromCam = getPacket();
    if(!(fromCam.cmd2Token == ACK && fromCam.parameter1 == INITIAL))
        return NO_INITIAL;// if reconfiguration was not successful

    camera2ComPort.baudrate(115200); // change UART baud rate

    /* specify image quality */
    toCam = newCamera2Packet(); // initialize packet
    toCam.cmd2Token = QUALITY;  // configure picture quality
    toCam.parameter1 = 0x00;   // to be the best
    sendPacket(toCam);

    /* if camera acknowledges change, then get an image */
    fromCam = getPacket();
    if(!(fromCam.cmd2Token == ACK && fromCam.parameter1 == QUALITY))
        return NO_QUALITY;// if reconfiguration was not successful

    /* get an image */
    toCam = newCamera2Packet(); // initialize packet
    toCam.cmd2Token = GET_PIC;  // configure picture quality
    toCam.parameter1 = 0x05;   // get full size image
    sendPacket(toCam);

    /* if camera acknowledges request, then retrieve image data */
    fromCam = getPacket();
    if(!(fromCam.cmd2Token == ACK && fromCam.parameter1 == GET_PIC))
        return NO_GET_PIC; // if request was not successful

    /* get image size */
    fromCam = getPacket();
    if(!(fromCam.cmd2Token == DATA))
        return NO_DATA; // if request was not successful

    /* read data size */
    picsize = bytesToGet = fromCam.parameter2 + fromCam.parameter3 * 0x100LL + fromCam.parameter4 * 0x10000LL;
    int errorCounter = 0;

    /* store the image */
    while(bytesToGet > 0) // until all bytes retrieved
    {
        tempSize = camera2ComPort.receive(sysBuffer, SYS_BUFFER_SIZE); // receive the bytes
        //imageFile.write(tempBuffer,tempSize); // store the bytes
        FSfwrite(sysBuffer, sizeof(char), tempSize, imageFile2);
        bytesToGet -= tempSize; // update bytes remaining

        // watch dog counter - bytes are sometimes lost...so loop will hang
        errorCounter = tempSize ? 0: errorCounter + 1;
        
        // if anytime the receive buffer is larger than 0, then reset the error counter
        if (tempSize > 0)
            errorCounter = 0;

        if(errorCounter > 200)
            break;
    }
    pause(30);
    
    if(bytesToGet)
        return LOST_DATA; // not all camera data was gathered
    else
        return NO_ERROR; // successful

}

static void setPowerOutput2(Boolean desiredOutputState)
{
   _TRISE0 = 0;    /* configure port as output */
   _RE0 = desiredOutputState; /* set the output */
}

static void turnOff2(void)
{
    setPowerOutput2(OFF);   // turn module power off
}

static void turnOn2(void)
{
    setPowerOutput2(ON);   // turn module power on
}

static void initialize2(void)
{
    setPowerOutput2(OFF);
    camera2ComPort = uart2;
    imageFile2 = NullPtr;
}
static CameraError retrievePic2(String imgName)
{
    setPowerOutput2(ON);
    /**
     * Increasing Power On Period
     *
     * Although the data sheet says that the camera needs ~1.5 sec to start-up,
     * in practice 2 sec makes for a much more reliable situation.
     */
    wait(2000);//1500);

    // initialize camera and image storage
    camera2ComPort.init();
    camera2ComPort.baudrate(14400);
    //imageFile = getFileStream();
    imageFile2 = FSfopen(imgName, "w");
    CameraError error = NO_FILE;

    //if(imageFile.open)
    if(imageFile2)
    {
        //FSfclose(FSfopen(imgName, "w")); // erase file
        //imageFile.open(imgName); // open file
        error = getPicture2();
        FSfclose(imageFile2);
        imageFile2 = NullPtr;
        //imageFile.close();
    }

    //imageFile.free();

    setPowerOutput2(OFF);
    wait(1000);

    return error;
}

Boolean getStatus2(void)
{
    return TRUE;
}

const Camera camera2 = {
    .getPix = retrievePic2,
    .on     = turnOn2,
    .off    = turnOff2,
    .init   = initialize2,
    .isOk   = getStatus2
};

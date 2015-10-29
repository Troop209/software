#include "camera.h"
#include "file.h"
#include "dataLog.h"

/*  BUFFER_SIZE Information
 *
 *  BUFFER_SIZE defines the size of buffers used throughout the serial module.
 */
#define RX_BUFFER_SIZE    0x0800
#define TX_BUFFER_SIZE    0x0040


/**  TX_buffer Information
 *
 *  The TX_buffer holds bytes that are waiting to be sent out the UART module.
 *  This global circular buffer is serviced in the background via interrupts.
 *  The buffer is indexed via TX_head and TX_next.
 */
static Byte TX_buffer[TX_BUFFER_SIZE] = {0};
static unsigned volatile TX_head = 0;
static unsigned volatile TX_next = 0;

/**  RX_buffer Information
 *
 *  The RX_buffer holds bytes that have been received by the UART module. This
 *  global circular buffer is serviced in the background via interrupts. The
 *  buffer is indexed via RX_head and RX_next.
 */
static Byte RX_buffer[RX_BUFFER_SIZE] = {0};
static unsigned volatile RX_head = 0;
static unsigned volatile RX_next = 0;

static void initUart(void)
{
    _U1RXIE = 0;    /* disable UART Rx interrupt */
    _U1TXIE = 0;    /* disable UART Tx interrupt */

    U1MODEbits.UARTEN = OFF;  /* disable the UART module while configuring */
    U1MODEbits.USIDL = OFF;   /* do not stop in idle mode */
    U1MODEbits.IREN = OFF;    /* disable IrDA encode and decode */
    U1MODEbits.UEN = 0b00;    /* manual hardware flow control */
    U1MODEbits.WAKE = ON;     /* wake up on detection of start bit */
    U1MODEbits.LPBACK = OFF;  /* disable loopback mode */
    U1MODEbits.ABAUD = OFF;   /* disable baud rate auto-detect */
    U1MODEbits.RXINV = OFF;   /* do not invert Rx idle state */
    U1MODEbits.BRGH = 1;      /* high speed baud rate */
    U1MODEbits.PDSEL = 0b00;  /* 8-bit data, no parity */
    U1MODEbits.STSEL = 0;     /* 1 stop bit */
    U1STAbits.UTXISEL1 = 0b1; /* interrupt when Tx buffer is empty */
    U1STAbits.UTXISEL0 = 0b0; /* interrupt when Tx buffer is empty */
    U1STAbits.UTXINV = OFF;   /* do not invert Tx idle bit*/
    U1STAbits.UTXBRK = OFF;   /* disable sync break transmission */
    U1STAbits.UTXEN = YES;    /* enable UART transmitter */
    U1STAbits.URXISEL = 0b00; /* interrupt when character received */
    U1STAbits.OERR = 0;       /* clear Rx overrun error flag */

    U1BRG = ((FCY/9600)/4) - 1;  /* calculate value for baud register */

    /* configure pin direction */
    _TRISG8 = 0;    /* TX -> output */
    _TRISG7 = 1;    /* RX -> input */

    /* map modules to pins */
    __builtin_write_OSCCONL(OSCCON & 0xBF);     /* unlock peripheral pin select registers */
    _RP19R = 3;                                 /* Remmapable pin 19 is UART1 transmit */
    _U1RXR = 26;                                /* UART1 recieve mapped to remmapable pin 26 */
    __builtin_write_OSCCONL(OSCCON | 0x40);     /* lock peripheral pin select registers */

    /* clear interrupts so they don't immediately interrupt */
    _U1RXIF = OFF; /* clear Rx interrupt flag */
    _U1TXIF = OFF; /* clear Tx interrupt flag */

    _U1RXIP = 6; /* set Rx interrupt priority */
    _U1TXIP = 4; /* set Tx interrupt priority */

    U1MODEbits.UARTEN = ON; /* enable the UART module now that its configured */
    /* Note: UART Tx cannot be enabled until the UART module itself has been enabled. */
    U1STAbits.UTXEN = YES;    /* enable UART transmitter */

    _U1RXIE = 1;    /* enable UART Rx interrupt */
}

/**  getTxSize() Information
 *
 *  @Returns number of bytes in TX buffer.
 */
inline static unsigned getTxSize(void)
{
    return  TX_next - TX_head
            + (TX_head > TX_next ? TX_BUFFER_SIZE: 0);
}

/**  getRxSize() Information
 *
 *  @Returns number of bytes in RX buffer.
 */
inline static unsigned getRxSize(void)
{
    return  RX_next - RX_head
            + (RX_head > RX_next ? RX_BUFFER_SIZE: 0);
}

/**  getTxSpace() Information
 *
 *  @Returns room remaining TX buffer.
 */
inline static unsigned getTxSpace(void)
{
    return  TX_head - TX_next - 1
            + (TX_head <= TX_next ? TX_BUFFER_SIZE: 0);
}

/**  UART Receive Interrupt
 *
 *  As the microcontroller receives data from the connected peripheral, it will
 *  be queued in order to be processed and interpreted later.
 */
_ISR_ _U1RXInterrupt(void)
{

    while(U1STAbits.URXDA) /* while data is available */
    {
        /* copy byte to buffer */
        RX_buffer[RX_next++] = U1RXREG;

        /* protect against rollover */
        RX_next %= RX_BUFFER_SIZE;

        /* accommodate overflow */
        if(RX_head == RX_next)
            RX_head = (RX_head + 1) % RX_BUFFER_SIZE;
    }

    /* clear the interrupt flag */
    _U1RXIF = OFF; /* clear Rx interrupt flag */

}

/** Empties the receive buffer */
inline static void clearRx(void)
{
    RX_head = RX_next;
}

/**  UART Transmit Interrupt
 *
 *  As the microcontroller is able to sent data to the connected peripheral, it
 *  is sent out an removed from the transmit queue until data. This process is
 *  taken care of in the background by the interrupt. To send data, add it to
 *  the transmit queue.
 */
_ISR_ _U1TXInterrupt (void)
{
    /* clear the interrupt flag */
    _U1TXIF = OFF; /* clear Tx interrupt flag */

    /* transmit if transmit queue has room and buffer is not empty */
    while (!(U1STAbits.UTXBF) && (TX_head != TX_next))
    {
        U1TXREG = TX_buffer[TX_head++];
        TX_head %= TX_BUFFER_SIZE;
    }
}

/**  sendFrom() Information
 *
 *  @Purpose
 *      Sends the specified number of bytes from the pointed to string to the
 *      connected peripheral.
 *  @Parameters
 *      startOfData: pointer to string of bytes to send
 *      bytesToSend: number of bytes to send from the byte array
 *  @Returns
 *      Boolean indicating weather or not bytes were sent to the connected
 *      peripheral.
 */
static boolean sendFrom(const Byte* startOfData, unsigned bytesToSend)
{
    /* can't send nonexistent data or no data */
    if (!startOfData || !bytesToSend)
        return FALSE;

    /* make sure room is available in buffer */
    while(getTxSpace() < bytesToSend);

    /* pause background sending of data */
    _U1TXIE = 0;    /* disable UART Tx interrupt */

    /* send bytes while bytes still need to be sent */
    while(bytesToSend--)
    {
        TX_buffer[TX_next++] = *startOfData;

        /* circular queue rollover protection */
        TX_next %= TX_BUFFER_SIZE;

        /* accommodate overflow */
        if(TX_head == TX_next)
            TX_head = (TX_head + 1) % TX_BUFFER_SIZE;

        ++startOfData;
    }

    /* continue with background transmitting */
    _U1TXIE = YES;    /* enable UART Tx interrupt */

    /* force transmit process to start if haven't started already */
    _U1TXIF = YES;    /* trigger UART Tx interrupt */

    return TRUE;
}

inline static void startRx(void)
{
    clearRx();   /* empty the Rx buffer */
    _U1RXIE = 1; /* enable Rx interrupt */
}

inline static void setBaudRate(unsigned long long desiredBaudRate)
{
    U1BRG = ((FCY/desiredBaudRate)/4) - 1;  /* calculate value for baud register */
}

/**  copyFrom() Information
 *
 *  @Purpose
 *      Reads the specified number of bytes from the receive buffer and stores them at
 *      the pointed to memory location.
 *  @Parameters
 *      startOfStorage: pointer to location to store string of bytes received
 *      maxBytesToRead: maximum number of bytes to copy from the byte array
 *  @Returns
 *      The number of bytes copied from the receive buffer.
 */
static int copyFrom(Byte* startOfStorage, int maxBytesToRead)
{
    if(!startOfStorage)
        return 0;

    _U1RXIE = 0;    /* disable UART Rx interrupt */

    int head = RX_head,
        next = RX_next,
        count = 0;

    while(maxBytesToRead-- && (head != next))
    {
        startOfStorage[count++] = RX_buffer[head++];
        head %= RX_BUFFER_SIZE;
    }

    _U1RXIE = 1;    /* enable UART Rx interrupt */

    return count;
}

/**  readFrom() Information
 *
 *  @Purpose
 *      Retrieves the specified number of bytes from the receive buffer and
 *       stores them at the pointed to memory location.
 *  @Parameters
 *      startOfStorage: pointer to location to store string of bytes received
 *      maxBytesToRead: maximum number of bytes to read from the byte array
 *  @Returns
 *      The number of bytes read from the receive buffer.
 */
static int receiveFrom(Byte* startOfStorage, int maxBytesToRead)
{
    _RD6 = 1;
    if(!startOfStorage)
        return 0;

    int count = 0;

    while(maxBytesToRead-- && (RX_head != RX_next))
    {
        startOfStorage[count++] = RX_buffer[RX_head++];
        RX_head %= RX_BUFFER_SIZE;
    }

   _RD6 = 0;

    return count;
}

/**  dropFrom() Information
 *
 *  @Purpose
 *      Drops the specified number of bytes from the receive buffer. Used with
 *      copyFrom() to get data from the receive buffer.
 *  @Parameters
 *      bytesToDrop: number of bytes to drop from the receive buffer
 *  @Returns
 *      nothing
 */
static void dropFrom(int bytesToDrop)
{
    if(bytesToDrop <= 0)
        return;
    _U1RXIE = 0;    /* disable UART Rx interrupt */
    RX_head = (RX_head + bytesToDrop) % RX_BUFFER_SIZE;
    if(RX_head > RX_next)
        RX_head = RX_next;
    _U1RXIE = 1;    /* enable UART Rx interrupt */
}

/**  flush() Information
 *
 *  @Purpose
 *      Drops all bytes from the receive buffer.
 *  @Parameters
 *      none
 *  @Returns
 *      nothing
 */
static void flush(void)
{
     RX_head = RX_next;
}

const CameraComPort cameraComPort = {baudrate:setBaudRate,
                                     copy:copyFrom,
                                     drop:dropFrom,
                                     flush:flush,
                                     size:getRxSize,
                                     send:sendFrom,
                                     init:initUart,
                                     receive:receiveFrom};

/**
 * CameraPacket Information
 *
 * The C329-UART camera communicates using the following command set:
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
    Uint8 cmdToken;
    Uint8 parameter1;
    Uint8 parameter2;
    Uint8 parameter3;
    Uint8 parameter4;
  };
} CameraPacket;

/**
 * CmdToken Information
 *
 * The Cmd Token tells the camera which action to execute. The first 3 bytes of each Cmd Token
 * are all 0xFF, so they can be kept constant. The fourth byte corresponds to the action to
 * perform.
 */
typedef enum CmdToken {
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
} CmdToken;


static CameraPacket newCameraPacket(void)
{
    static const CameraPacket emptyPacket = {{0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00}};
    return emptyPacket;
}


static void sendPacket(CameraPacket packetToSend)
{
    cameraComPort.send(packetToSend.raw,8);
}

static CameraPacket getPacket(void)
{
    CameraPacket inPacket = newCameraPacket();
    while(cameraComPort.size() < 8);
    cameraComPort.receive(inPacket.raw,8);
    return inPacket;
}

static boolean initCamera(void)
{
    CameraPacket toCam = newCameraPacket(),
                 fromCam = newCameraPacket();

    toCam.cmdToken = SYNC;

    /* try and get sync with camera */
    while(!(fromCam.cmdToken == ACK && fromCam.parameter1 == SYNC)) // until ACK received
    {
        cameraComPort.flush();
        sendPacket(toCam); // send SYNC command
        fromCam = getPacket(); // get reply
        pause(5);
    }

    /* acknowledge camera's sync request */
    toCam.cmdToken = ACK; // acknowledge
    toCam.parameter1 = SYNC; // the sync

    while(!(fromCam.cmdToken == SYNC)) // wait for camera SYNC request
        fromCam = getPacket();
    sendPacket(toCam);


    /* configure camera */
    toCam.cmdToken = INITIAL; // initial configuration
    toCam.parameter1 = 0x04; // 115,200 baud rate
    toCam.parameter2 = 0x87; // compress color
    toCam.parameter3 = 0x01; // 80x60 preview resolution
    toCam.parameter4 = 0x07; // 640x480 compression resolution
    sendPacket(toCam);

    /* if camera acknowledges changes, change the UART baud rate */
    fromCam = getPacket();
    if(fromCam.cmdToken == ACK && fromCam.parameter1 == INITIAL) // if reconfiguration was successful
    {
        cameraComPort.baudrate(115200); // change UART baud rate
        return FALSE; // report that no errors occurred
    }

    return TRUE;

}
static FileStream imageFile;
static Byte tempBuffer[RX_BUFFER_SIZE];
static int tempSize = 0;
static Int32 bytesToGet = 0;
static Int32 picsize = 0;
static CameraPacket toCam, fromCam;
static boolean getPicture(void)
{
    toCam = newCameraPacket(),
    fromCam = newCameraPacket();

    toCam.cmdToken = SYNC;

    /* try and get sync with camera */
    while(!(fromCam.cmdToken == ACK && fromCam.parameter1 == SYNC)) // until ACK received
    {
        cameraComPort.flush();
        sendPacket(toCam); // send SYNC command
        fromCam = getPacket(); // get reply
        pause(5);
    }

    /* acknowledge camera's sync request */
    toCam = newCameraPacket(); // initialize packet
    toCam.cmdToken = ACK; // acknowledge
    toCam.parameter1 = SYNC; // the sync

    while(!(fromCam.cmdToken == SYNC)) // wait for camera SYNC request
        fromCam = getPacket();
    sendPacket(toCam);

    /* configure camera */
    toCam = newCameraPacket(); // initialize packet
    toCam.cmdToken = INITIAL; // initial configuration
    toCam.parameter1 = 0x04; // 115,200 baud rate
    toCam.parameter2 = 0x87; // compress color
    toCam.parameter3 = 0x01; // 80x60 preview resolution
    toCam.parameter4 = 0x07; // 640x480 compression resolution
    sendPacket(toCam);

    /* if camera acknowledges changes, change the UART baud rate */
    fromCam = getPacket();
    if(!(fromCam.cmdToken == ACK && fromCam.parameter1 == INITIAL))
        return TRUE;// if reconfiguration was not successful

    cameraComPort.baudrate(115200); // change UART baud rate

    /* specify image quality */
    toCam = newCameraPacket(); // initialize packet
    toCam.cmdToken = QUALITY; // configure picture quality
    toCam.parameter1 = 0x00; // to be the best
    sendPacket(toCam);

    /* if camera acknowledges change, then get an image */
    fromCam = getPacket();
    if(!(fromCam.cmdToken == ACK && fromCam.parameter1 == QUALITY))
        return TRUE;// if reconfiguration was not successful

    /* get an image */
    toCam = newCameraPacket(); // initialize packet
    toCam.cmdToken = GET_PIC; // configure picture quality
    toCam.parameter1 = 0x05; // get full size image
    sendPacket(toCam);

    /* if camera acknowledges request, then retrieve image data */
    fromCam = getPacket();
    if(!(fromCam.cmdToken == ACK && fromCam.parameter1 == GET_PIC))
        return TRUE;// if request was not successful

    /* get image size */
    fromCam = getPacket();
    if(!(fromCam.cmdToken == DATA))
        return TRUE;// if request was not successful

    /* read data size */
    picsize = bytesToGet = fromCam.parameter2 + fromCam.parameter3 * 0x100LL + fromCam.parameter4 * 0x10000LL;

    int cnt = 0;

    /* store the image */
    while(bytesToGet > 0) // until all bytes retrieved
    {
        tempSize = cameraComPort.receive(tempBuffer, RX_BUFFER_SIZE); // receive the bytes
        imageFile.write(tempBuffer,tempSize); // store the bytes
        bytesToGet -= tempSize; // update bytes remaining

        cnt = tempSize ? 0: cnt + 1;

        if(cnt > 200)
            break;

    }

    /* acknowledge that data was received */
    toCam = newCameraPacket(); // initialize packet
    toCam.cmdToken = ACK; // notify the camera of successful
    toCam.parameter1 = DATA; // data retrieval
    /* not needed */
    //sendPacket(toCam);
    pause(30);
    return TRUE;

}

static int retrievePic(void)
{

    _TRISD6 = 0;
    _TRISB10 = 0;
    _PCFG10  = 1;

    _RD6 = 0;
    _RB10 = 0;

    static boolean inited = NO;
    if(!inited)
    {
        // initialize camera and image storage
        cameraComPort.init();
        cameraComPort.baudrate(14400);

        imageFile = getFileStream();
        initCamera();

    }

    inited = YES;

    // get first picture
    static int cnt = 0;
    char imgName[12] = {"0x0000.jpg"};
    char toHex[] = {"0123456789ABCDEF"};
    imgName[2] = toHex[(cnt & 0xF000) >> 12];
    imgName[3] = toHex[(cnt & 0x0F00) >> 8];
    imgName[4] = toHex[(cnt & 0x00F0) >> 4];
    imgName[5] = toHex[(cnt & 0x000F) >> 0];

    imageFile.open(imgName);
    pause(100);
    getPicture();
    imageFile.close();

    dataLog.add("image = ", cnt);
    ++cnt;

    return 0;
}

static void setPowerOutput(boolean desiredOutputState)
{
    _TRISE1 = 0;    /* configure port as output */
    _RE1 = desiredOutputState; /* set the output (active high) */
}

static void turnOff(void)
{
    setPowerOutput(OFF);   // turn module power off
}

static void turnOn(void)
{
    setPowerOutput(ON);   // turn module power on
}

static boolean initIt(void)
{
        // initialize camera and image storage
        cameraComPort.init();
        cameraComPort.baudrate(14400);

        imageFile = getFileStream();
        return (initCamera());
    
}

const Camera camera = {getPic:retrievePic, on:turnOn, off:turnOff};
const XCamera xcamera =
    {getPic:retrievePic, on:turnOn, off:turnOff, init:initIt};


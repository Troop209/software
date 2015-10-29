#include "system.h"

/**
 * Wrapper that provides serial I/O for the C329 camera using UART
 */
typedef struct {
    /**
     * Configures the baud rate
     * @param baudrate - the desired baud rate (in bits/sec) as a positive integer
     */
    void (*baudrate)(unsigned long long baudrate);
    /**
     * Reads the specified number of bytes from the UART port's receive buffer
     *      and stores them at the pointed to memory location.
     * @param storageBuffer - pointer to the byte array to store the data copied
     *                        from the UART port
     * @param maxBytesToRead - maximum number of bytes to copy from the UART port
     *                         and store in the storageBuffer
     * @return number of bytes copied from the UART port receive buffer
     * @Note This function does not remove data from the UART port buffer.
     *       Bytes copied from the buffer remain until dropped.
     */
    int (*copy)(Byte* storageBuffer, int maxBytesToRead);
    /**
     * Removes bytes from the UART port's receive buffer
     * @param bytesToDrop - the desired number of bytes to drop from the receive buffer
     */
    void (*drop)(int bytesToDrop);
    /**
     * Removes all bytes from the UART port's receive buffer
     */
    void (*flush)(void);
    /**
     * Counts the number of bytes UART port's receive buffer
     * @return the number of bytes in the receive buffer
     */
    unsigned (*size)(void);
    /**
     * Sends the specified number of bytes from the pointed to string to the
     *      connected peripheral.
     * @param startOfData - pointer to string of bytes to send
     * @param bytesToSend - number of bytes to send from the byte array
     * @return <code>boolean</code> indicating weather or not bytes were sent to
     *       the connected peripheral.
     */
    boolean (*send)(const Byte* startOfData, unsigned bytesToSend);
    /**
     * Pulls the specified number of bytes from the UART port's receive buffer
     *      and stores them at the pointed to memory location.
     * @param storageBuffer - pointer to the byte array to store the data read
     *                        from the UART port
     * @param maxBytesToRead - maximum number of bytes to pull from the UART port
     *                         and store in the storageBuffer
     * @return number of bytes read from the UART port receive buffer
     * @Note This function removes data from the UART port buffer.
     */
    int (*receive)(Byte* storageBuffer, int maxBytesToRead);
    /**
     * Initializes the camera UART software module
     */
    void (*init)(void);


} CameraComPort;

extern const CameraComPort cameraComPort;

/**
 * Wrapper for the C329-UART camera
 */
typedef struct {
    /**
     * Takes a picture and saves it to the SD card using the passed filename
     * @param filename - as a <code>String</code> to store the image as
     */
    //int (*getPic)(String filename);
    int (*getPic)(void);
    /**
     * Initializes the Camera software module
     */
    //void (*init)(void);

    /**
     * Turns on the camera
     */
    void (*on)(void);
    /**
     * Turns off the camera
     */
    void (*off)(void);

} Camera;

extern const Camera camera;

/**
 * Wrapper for the C329-UART camera
 */
typedef struct {
    /**
     * Takes a picture and saves it to the SD card using the passed filename
     * @param filename - as a <code>String</code> to store the image as
     */
    //int (*getPic)(String filename);
    int (*getPic)(void);
    /**
     * Initializes the Camera software module
     */
    boolean (*init)(void);

    /**
     * Turns on the camera
     */
    void (*on)(void);
    /**
     * Turns off the camera
     */
    void (*off)(void);

} XCamera;

extern const XCamera xcamera;

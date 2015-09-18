#ifndef FILE_H
#define FILE_H

#include "system.h"
#include "sdcard.h"

typedef struct FileStream FileStream;
/**
 * Wrapper that provides file input and output
 */
struct FileStream {
    /**
     * Opens the file in read/write append mode
     * @param fileName - name of the file to open (must be a String constant - i.e. "a.txt")
     * @return <code>FALSE</code> if successful; <code>TRUE</code> if unsuccessful
     */
    int (*open)(const String fileName);
    /**
     * Closes the file
     * @return <code>FALSE</code> if successful; <code>TRUE</code> if unsuccessful
     */
    int (*close)(void);
    /**
     * Write data to the file
     * @param dataBuffer - pointer to the byte array to write to the file
     * @param bytesToWrite - number of bytes to write to the file from dataBuffer
     * @return number of bytes written to the file
     */
    int (*write)(Byte* dataBuffer, int bytesToWrite);
    /**
     * Read data from the file
     * @param storageBuffer - pointer to the byte array to store the data read from the file
     * @param bytesToRead - number of bytes to read from the file and store in the storageBuffer
     * @return number of bytes read from the file
     * #Note Data is read from the beggining of the file after a write takes place
     *       or the end-of-file is reached. Sequential reads continue where the
     *       previous read left off.
     */
    int (*read)(Byte* storageBuffer, int bytesToRead);
};

FileStream getFileStream(void);

extern const FileStream file1, file2, file3;

#endif /* FILE_H */

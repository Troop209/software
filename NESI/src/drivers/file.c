#include "file.h"

/*------------------------------------ 1 ----------------------------*/
/* file pointer for this module */
static FSFILE* fileptr1 = NullPtr;

/* close this file */
static int close1(void)
{
    if(!FSfclose(fileptr1))               /* close file and */
        return (int)(fileptr1 = NullPtr); /* unassign pointer */
    return EOF;                           /* otherwise, report error */
}

/* check if file is open */
static boolean isOpen1(void)
{
    /* file pointer is null until opened */
    return NullPtr != fileptr1;
}

/* open file for reading and writing */
static int open1(const String fileName)
{
    if(!fileptr1)                           /* if file not yet created */
        fileptr1 = FSfopen(fileName, "a+"); /* open in r/w append mode */
    return NullPtr == fileptr1;             /* file null upon failure */
}

/* write to this module's file */
static int write1(Byte* dataBuffer, int bytesToWrite)
{
    return  FSfwrite((void *)dataBuffer, sizeof(Byte), bytesToWrite, fileptr1);
}

/* read from the beginning of this module's file */
static int read1(Byte* storageBuffer, int bytesToRead)
{
    if(FSfeof(fileptr1))  /* if currently at the end of the file */
        if(FSfseek(fileptr1, 0, SEEK_SET)) /* move to the beginning */
            return EOF; /* report error if cannot read from the beginning */

    /* return the number of bytes read from the file */
    return FSfread(storageBuffer, sizeof(Byte), bytesToRead, fileptr1);
}

const FileStream file1 = {open:open1, close:close1, read:read1, write:write1};

/*------------------------------------ 2 ----------------------------*/
/* file pointer for this module */
static FSFILE* fileptr2 = NullPtr;

/* close this file */
static int close2(void)
{
    if(!FSfclose(fileptr2))               /* close file and */
        return (int)(fileptr2 = NullPtr); /* unassign pointer */
    return EOF;                           /* otherwise, report error */
}

/* check if file is open */
static boolean isOpen2(void)
{
    /* file pointer is null until opened */
    return NullPtr != fileptr2;
}

/* open file for reading and writing */
static int open2(const String fileName)
{
    if(!fileptr2)                           /* if file not yet created */
        fileptr2 = FSfopen(fileName, "a+"); /* open in r/w append mode */
    return NullPtr == fileptr2;             /* file null upon failure */
}

/* write to this module's file */
static int write2(Byte* dataBuffer, int bytesToWrite)
{
    return  FSfwrite((void *)dataBuffer, sizeof(Byte), bytesToWrite, fileptr2);
}

/* read from the beginning of this module's file */
static int read2(Byte* storageBuffer, int bytesToRead)
{
    if(FSfeof(fileptr2))  /* if currently at the end of the file */
        if(FSfseek(fileptr2, 0, SEEK_SET)) /* move to the beginning */
            return EOF; /* report error if cannot read from the beginning */

    /* return the number of bytes read from the file */
    return FSfread(storageBuffer, sizeof(Byte), bytesToRead, fileptr2);
}

const FileStream file2 = {open:open2, close:close2, read:read2, write:write2};

/*------------------------------------ 3 ----------------------------*/
/* file pointer for this module */
static FSFILE* fileptr3 = NullPtr;

/* close this file */
static int close3(void)
{
    if(!FSfclose(fileptr3))               /* close file and */
        return (int)(fileptr3 = NullPtr); /* unassign pointer */
    return EOF;                           /* otherwise, report error */
}

/* check if file is open */
static boolean isOpen3(void)
{
    /* file pointer is null until opened */
    return NullPtr != fileptr3;
}

/* open file for reading and writing */
static int open3(const String fileName)
{
    if(!fileptr3)                           /* if file not yet created */
        fileptr3 = FSfopen(fileName, "a+"); /* open in r/w append mode */
    return NullPtr == fileptr3;             /* file null upon failure */
}

/* write to this module's file */
static int write3(Byte* dataBuffer, int bytesToWrite)
{
    return  FSfwrite((void *)dataBuffer, sizeof(Byte), bytesToWrite, fileptr3);
}

/* read from the beginning of this module's file */
static int read3(Byte* storageBuffer, int bytesToRead)
{
    if(FSfeof(fileptr3))  /* if currently at the end of the file */
        if(FSfseek(fileptr3, 0, SEEK_SET)) /* move to the beginning */
            return EOF; /* report error if cannot read from the beginning */

    /* return the number of bytes read from the file */
    return FSfread(storageBuffer, sizeof(Byte), bytesToRead, fileptr3);
}

const FileStream file3 = {open:open3, close:close3, read:read3, write:write3};

const static FileStream fileNull = {NullPtr};

/** Returns a File object */
FileStream getFileStream(void) // TODO - write proper wrapper
{
    sdcard.init();

    if(sdcard.isOk())  // no files if FS uninitialized
    {
        if (!isOpen1())
            return file1;
        else if (!isOpen2())
            return file2;
        else if (!isOpen3())
            return file3;
    }

    return fileNull;
}


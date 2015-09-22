#include "datalog.h"
#include "file.h"

static FileStream dataFile;

static void initialize(void)
{
    dataFile = getFileStream();
}

static boolean isInitialized(void)
{
    return !!dataFile.open;
}

static void logValue(String label, unsigned value)
{
    // open file
    dataFile.open("dataLog.txt");

    // determine label length
    int size = 0;
    String temp = label;

    for(temp = label; *temp; ++temp)
    {
        ++size;
    }

    // write label
    dataFile.write(label, size);

    // convert value
    char dataString[8] = {"0x0000\n"};
    static const char toHex[] = {"0123456789ABCDEF"};

    dataString[2] = toHex[(value & 0xF000) >> 12];
    dataString[3] = toHex[(value & 0x0F00) >> 8];
    dataString[4] = toHex[(value & 0x00F0) >> 4];
    dataString[5] = toHex[(value & 0x000F)];

    // write value
    dataFile.write(dataString, 7);

    // close file
    dataFile.close();
}

const DataLog dataLog = {init:initialize, add:logValue, isOk:isInitialized, file:&dataFile};


#include "../core/system.h"
#include "../core/file.h"

/* Write something to RB11 */
static void fileappend()
{

    char buffer[20] = "Hello There!";
    file1.open("NormTest.txt");
    file1.write((Byte *)buffer, 12);
    file1.close();
} 


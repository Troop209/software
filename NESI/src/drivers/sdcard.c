#include "sdcard.h"

static boolean initialized = FALSE;

/* initialize SD card */
static void initialize(void)
{

    // configure SD-SPI Chip Selects
    _TRISD2 = 0;  // configure primary chip select as output
    _RD2 = 1;     // deselect prmary SD card
    _TRISD9 = 0;  // configure secondary chip select as output
    _RD9 = 1;     // deselect secondary SD card

    /* do not reinitialize if already configured */
    if(initialized)
        return;

    /* initialize the SPI - SD card used in MMC mode */
    __builtin_write_OSCCONL(OSCCON & 0xBF);
    _SDI1R = 12; // data in (MISO)
    _RP11R = 7;  // data out (MOSI)
    _RP3R = 8;   // clock ouut (SCLK)
    __builtin_write_OSCCONL(OSCCON | 0x40);

    /*  Detecting the SD Card
     *
     *  The SD card detect switch is active low (reads 0 when the card is inserted).
     *  To prevent false detects, the detect line will be pulled high using the
     *  internal pull-up resistor. When the card is not present, the floating line
     *  will be pulled high. Once the card is inserted, the line will be driven low.
     */
    _CN53PUE = 1;
    _CN49PUE = 1;
    _CN56PUE = 1;
    _CN55PUE = 1;
    _CN54PUE = 1;
    _CN16PUE = 1;
    _CN61PUE = 1;

    /* FSInit initializes the MDD File System used to access files on the SD card. */
    initialized = FSInit();
}

static boolean isInitialized(void)
{
    return initialized;
}

const Sdcard sdcard = {init:initialize, isOk:isInitialized};

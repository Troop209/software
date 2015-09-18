#ifndef USB_H
#define USB_H

#include "../dev/GenericTypeDefs.h"
#include "../dev/Compiler.h"
#include "../usb/usb_config.h"
#include "../usb/usb_common.h"
#include "../usb/usb_ch9.h"
#include "../usb/usb_device.h"
#include "../usb/usb_hal.h"

#include "system.h"

/**
 * Wrapper for USB
 */
typedef struct {
    /**
     * Initialize the USB stack.
     */
    void (*init)(void);
    /**
     * Connect to the USB host.
     */
    void (*connect)(void);
    /**
     * Disconnect from the USB host.
     */
    void (*disconnect)(void);
    /**
     * Disconnect from the USB host.
     */
    void (*eject)(void);
    /**
     * Process pending requests and data transfers.
     */
    void (*process)(void);
} Usb;

extern const Usb usb;

#endif /* USB_H */

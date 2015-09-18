#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include <p24fj256gb106.h>

/*  Remappable Pin Information
 *
 *  As an additional level of safety, the device can be configured to prevent
 *  more than one write session to the RPINRx  and RPORx registers. The IOL1WAY
 *  (CW2<4>) Configuration bit blocks the IOLOCK bit from being cleared after
 *  it has been set once. If IOLOCK remains set, the register unlock procedure
 *  will not execute and the Peripheral Pin Select Control registers cannot be
 *  written to. The only way to clear the bit and re-enable peripheral
 *  remapping is to perform a device Reset.
 *
 *  Under normal operation, writes to the RPINRx and RPORx registers are not
 *  allowed. Attempted writes will appear to execute normally, but the contents
 *  of the registers will remain unchanged. To change these registers, they
 *  must be unlocked in hardware. The register lock is controlled by the IOLOCK
 *  bit (OSCCON<6>). Setting IOLOCK prevents writes to the control registers;
 *  clearing IOLOCK allows writes.
 *
 *  To set or clear IOLOCK, a specific command sequence must be executed:
 *     1.  Write 46h to OSCCON<7:0>.
 *     2.  Write 57h to OSCCON<7:0>.
 *     3.  Clear (or set) IOLOCK as a single operation.
 *  You can alternatively call unlockRemappablePins() (or lockRemappablePins())
 *  which uses the builtin function to unlock (or lock). Unlike the similar
 *  sequence with the oscillator’s LOCK bit, IOLOCK remains in one state
 *  changed. This allows all of the Peripheral Pin Selects to be configured
 *  with a single unlock sequence, followed by an update to all control
 *  registers, then locked with a second lock sequence.
 */

/*  Remappable Pin Lock and Unlock Macro Information
 *
 *  Before the remappable pins can be reassigned, they must first be unlocked.
 *  Use unlockRemappablePins() to allow the reconfiguration of the pins and use
 *  lockRemappablePins() once done in order to ensure that your is not
 *  accidentally changed later in the program.
 */
#define unlockRemappablePins()  (__builtin_write_OSCCONL(OSCCON & 0xBF))
#define lockRemappablePins()    (__builtin_write_OSCCONL(OSCCON | 0x40))


/*  INPUT_TO_[PERIPHERAL] and REMAPPABLE_PIN_[#] Information
 *
 *  The following INPUT_TO_[PERIPHERAL] definitions are used to map input
 *  peripherals to remappable pins. To use them, assign a REMAPPABLE_PIN_[#] to
 *  the desired peripherial.
 *
 *  ex.   // assign External Interrupt 1 to pin RP14 (pin 29) (Hour button)
 *        INPUT_TO_EXTERNAL_INTERRUPT_1 = REMAPPABLE_PIN_14;
 */
#define INPUT_TO_EXTERNAL_INTERRUPT_1       (_INT1R)
#define INPUT_TO_EXTERNAL_INTERRUPT_2       (_INT2R)
#define INPUT_TO_EXTERNAL_INTERRUPT_3       (_INT3R)
#define INPUT_TO_EXTERNAL_INTERRUPT_4       (_INT4R)
#define INPUT_TO_INPUT_CAPTURE_1            (_IC1R)
#define INPUT_TO_INPUT_CAPTURE_2            (_IC2R)
#define INPUT_TO_INPUT_CAPTURE_3            (_IC3R)
#define INPUT_TO_INPUT_CAPTURE_4            (_IC4R)
#define INPUT_TO_INPUT_CAPTURE_5            (_IC5R)
#define INPUT_TO_INPUT_CAPTURE_6            (_IC6R)
#define INPUT_TO_INPUT_CAPTURE_7            (_IC7R)
#define INPUT_TO_INPUT_CAPTURE_8            (_IC8R)
#define INPUT_TO_INPUT_CAPTURE_9            (_IC9R)
#define INPUT_TO_OUTPUT_COMPARE_FAULT_A     (_OCFAR)
#define INPUT_TO_OUTPUT_COMPARE_FAULT_B     (_OCFBR)
#define INPUT_TO_SPI1_CLOCK_INPUT           (_SCK1R)
#define INPUT_TO_SPI1_DATA_INPUT            (_SDI1R)
#define INPUT_TO_SPI1_SLAVE_SELECT_INPUT    (_SS1R)
#define INPUT_TO_SPI2_CLOCK_INPUT           (_SCK2R)
#define INPUT_TO_SPI2_DATA_INPUT            (_SDI2R)
#define INPUT_TO_SPI2_SLAVE_SELECT_INPUT    (_SS2R)
#define INPUT_TO_SPI3_CLOCK_INPUT           (_SCK3R)
#define INPUT_TO_SPI3_DATA_INPUT            (_SDI3R)
#define INPUT_TO_SPI3_SLAVE_SELECT_INPUT    (_SS3R)
#define INPUT_TO_TIMER2_EXTERNAL_CLOCK      (_T2CKR)
#define INPUT_TO_TIMER3_EXTERNAL_CLOCK      (_T3CKR)
#define INPUT_TO_TIMER4_EXTERNAL_CLOCK      (_T4CKR)
#define INPUT_TO_TIMER5_EXTERNAL_CLOCK      (_T5CKR)
#define INPUT_TO_UART1_CLEAR_TO_SEND        (_U1CTSR)
#define INPUT_TO_UART1_RECEIVE              (_U1RXR)
#define INPUT_TO_UART2_CLEAR_TO_SEND        (_U2CTSR)
#define INPUT_TO_UART2_RECEIVE              (_U2RXR)
#define INPUT_TO_UART3_CLEAR_TO_SEND        (_U3CTSR)
#define INPUT_TO_UART3_RECEIVE              (_U3RXR)
#define INPUT_TO_UART4_CLEAR_TO_SEND        (_U4CTSR)
#define INPUT_TO_UART4_RECEIVE              (_U4RXR)

#define REMAPPABLE_PIN_0        (0)
#define REMAPPABLE_PIN_1        (1)
#define REMAPPABLE_PIN_2        (2)
#define REMAPPABLE_PIN_3        (3)
#define REMAPPABLE_PIN_4        (4)
#define REMAPPABLE_PIN_6        (6)
#define REMAPPABLE_PIN_7        (7)
#define REMAPPABLE_PIN_8        (8)
#define REMAPPABLE_PIN_9        (9)
#define REMAPPABLE_PIN_10       (10)
#define REMAPPABLE_PIN_11       (11)
#define REMAPPABLE_PIN_12       (12)
#define REMAPPABLE_PIN_13       (13)
#define REMAPPABLE_PIN_14       (14)
#define REMAPPABLE_PIN_16       (16)
#define REMAPPABLE_PIN_17       (17)
#define REMAPPABLE_PIN_18       (18)
#define REMAPPABLE_PIN_19       (19)
#define REMAPPABLE_PIN_20       (20)
#define REMAPPABLE_PIN_21       (21)
#define REMAPPABLE_PIN_22       (22)
#define REMAPPABLE_PIN_23       (23)
#define REMAPPABLE_PIN_24       (24)
#define REMAPPABLE_PIN_25       (25)
#define REMAPPABLE_PIN_26       (26)
#define REMAPPABLE_PIN_27       (27)
#define REMAPPABLE_PIN_28       (28)
#define REMAPPABLE_PIN_29       (29)
#define REMAPPABLE_PIN_37       (37)


/*  OUTPUT_TO_REMAPPABLE_PIN_[#] and [OUTPUT PERIPHERAL] Information
 *
 *  The following OUTPUT_TO_REMAPPABLE_PIN_[#] definitions are used to map
 *  output peripherals to remappable pins. To use them, assign the desired
 *  output peripheral to the desired OUTPUT_TO_REMAPPABLE_PIN_[#].
 *
 *  ex.   // assign Output Compare 4 to pin RP29 (pin 30) (Second's LED)
 *        OUTPUT_TO_REMAPPABLE_PIN_29 = OUTPUT_COMPARE_4;
 */
#define OUTPUT_TO_REMAPPABLE_PIN_0      (_RP0R)
#define OUTPUT_TO_REMAPPABLE_PIN_1      (_RP1R)
#define OUTPUT_TO_REMAPPABLE_PIN_2      (_RP2R)
#define OUTPUT_TO_REMAPPABLE_PIN_3      (_RP3R)
#define OUTPUT_TO_REMAPPABLE_PIN_4      (_RP4R)
#define OUTPUT_TO_REMAPPABLE_PIN_6      (_RP6R)
#define OUTPUT_TO_REMAPPABLE_PIN_7      (_RP7R)
#define OUTPUT_TO_REMAPPABLE_PIN_8      (_RP8R)
#define OUTPUT_TO_REMAPPABLE_PIN_9      (_RP9R)
#define OUTPUT_TO_REMAPPABLE_PIN_10     (_RP10R)
#define OUTPUT_TO_REMAPPABLE_PIN_11     (_RP11R)
#define OUTPUT_TO_REMAPPABLE_PIN_12     (_RP12R)
#define OUTPUT_TO_REMAPPABLE_PIN_13     (_RP13R)
#define OUTPUT_TO_REMAPPABLE_PIN_14     (_RP14R)
#define OUTPUT_TO_REMAPPABLE_PIN_16     (_RP16R)
#define OUTPUT_TO_REMAPPABLE_PIN_17     (_RP17R)
#define OUTPUT_TO_REMAPPABLE_PIN_18     (_RP18R)
#define OUTPUT_TO_REMAPPABLE_PIN_19     (_RP19R)
#define OUTPUT_TO_REMAPPABLE_PIN_20     (_RP20R)
#define OUTPUT_TO_REMAPPABLE_PIN_21     (_RP21R)
#define OUTPUT_TO_REMAPPABLE_PIN_22     (_RP22R)
#define OUTPUT_TO_REMAPPABLE_PIN_23     (_RP23R)
#define OUTPUT_TO_REMAPPABLE_PIN_24     (_RP24R)
#define OUTPUT_TO_REMAPPABLE_PIN_25     (_RP25R)
#define OUTPUT_TO_REMAPPABLE_PIN_26     (_RP26R)
#define OUTPUT_TO_REMAPPABLE_PIN_27     (_RP27R)
#define OUTPUT_TO_REMAPPABLE_PIN_28     (_RP28R)
#define OUTPUT_TO_REMAPPABLE_PIN_29     (_RP29R)

#define COMPARATOR_1_OUTPUT         (1)
#define COMPARATOR_2_OUTPUT         (2)
#define UART1_TRANSMIT              (3)
#define UART1_REQUEST_TO_SEND       (4)
#define UART2_TRANSMIT              (5)
#define UART2_REQUEST_TO_SEND       (6)
#define SPI1_DATA_OUTPUT            (7)
#define SPI1_CLOCK_OUTPUT           (8)
#define SPI1_SLAVE_SELECT_OUTPUT    (9)
#define SPI2_DATA_OUTPUT            (10)
#define SPI2_CLOCK_OUTPUT           (11)
#define SPI2_SLAVE_SELECT_OUTPUT    (12)
#define OUTPUT_COMPARE_1            (18)
#define OUTPUT_COMPARE_2            (19)
#define OUTPUT_COMPARE_3            (20)
#define OUTPUT_COMPARE_4            (21)
#define OUTPUT_COMPARE_5            (22)
#define OUTPUT_COMPARE_6            (23)
#define OUTPUT_COMPARE_7            (24)
#define OUTPUT_COMPARE_8            (25)
#define UART3_TRANSMIT              (28)
#define UART3_REQUEST_TO_SEND       (29)
#define UART4_TRANSMIT              (30)
#define UART4_REQUEST_TO_SEND       (31)
#define SPI3_DATA_OUTPUT            (32)
#define SPI3_CLOCK_OUTPUT           (33)
#define SPI3_SLAVE_SELECT_OUTPUT    (34)
#define OUTPUT_COMPARE_9            (35)
#define COMPARATOR_3_OUTPUT         (36)

#endif  /* this is the end of the PERIPHERAL_H conditional compilation block */

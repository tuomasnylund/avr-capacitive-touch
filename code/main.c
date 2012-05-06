//standard libs
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//avr libs
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <avr/interrupt.h>

//LUFA stuff
#include <LUFA/Version.h>
#include <LUFA/Drivers/USB/USB.h>
#include "Descriptors.h"

//own stuff
#include "defines.h"
#include "touch.h"
#include "pwm16_rgb.h"

/**********************************************************
 * Function prototypes
 **********************************************************/

void initialize(void);

//uint16_t moving_average(uint16_t new);

//LUFA callbacks
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/**********************************************************
 * External variables
 **********************************************************/
extern volatile uint8_t USB_DeviceState;

/**********************************************************
 * Global variables
 **********************************************************/

static touch_channel_t btn1 = {
    .mux = 7,
    .port = &PORTF,
    .portmask = (1<<PF7),
    .min = 500,
    .max = 700
};

static touch_channel_t btn2 = {
    .mux = 6,
    .port = &PORTF,
    .portmask = (1<<PF6),
    .min = 500,
    .max = 700
};

static touch_channel_t btn3 = {
    .mux = 5,
    .port = &PORTF,
    .portmask = (1<<PF5),
    .min = 500,
    .max = 700
};

/** LUFA CDC Class driver interface configuration and state 
 *  information. This structure is passed to all CDC Class 
 *  driver functions, so that multiple instances of the same 
 *  class within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
{
    .Config =
    {
        .ControlInterfaceNumber         = 0,

        .DataINEndpointNumber           = CDC_TX_EPNUM,
        .DataINEndpointSize             = CDC_TXRX_EPSIZE,
        .DataINEndpointDoubleBank       = false,

        .DataOUTEndpointNumber          = CDC_RX_EPNUM,
        .DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
        .DataOUTEndpointDoubleBank      = false,

        .NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
        .NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
        .NotificationEndpointDoubleBank = false,
    },
};

/** Standard file stream for the CDC interface when set up,
 *  so that the virtual CDC COM port can be used like any
 *  regular character stream in the C APIs
 */
FILE USBSerialStream;

/**********************************************************
 * Main function
 **********************************************************/
int main(void){

    //variables
    uint16_t i;
    uint16_t sample[3];


    //initialize
    initialize();

    i = 0;
    //main loop
    while (1)
    {
        i++;
        if (i>10000){
            sample[0] = touch_measure(&btn1);
            sample[1] = touch_measure(&btn2);
            sample[2] = touch_measure(&btn3);
            pwm16_set_values(sample[0],sample[1],sample[2]);
            if(USB_DeviceState == DEVICE_STATE_Configured){

                fprintf(&USBSerialStream,"btn1: %u\t",sample[0]);
                fprintf(&USBSerialStream,"btn2: %u\t",sample[1]);
                fprintf(&USBSerialStream,"btn3: %u\r\n",sample[2]);
            }
            i=0;
            PORTE ^= (1<<PE2);
        }

        /**LUFA usb related tasks*/
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();

    }
    return 0;
}

/**********************************************************
 * Interrupt vectors
 **********************************************************/


/**********************************************************
 * Other functions
 **********************************************************/

/** Initializes all of the hardware. */
void initialize(void){

    CPU_PRESCALE(0x00);

    //LED
    DDRE |= (1<<PE2);

    //RGB LED
    DDRB |= (1<<PB5) | (1<<PB6);
    DDRC |= (1<<PC6);

    /** LUFA USB related inits */
	USB_Init();
	CDC_Device_CreateBlockingStream
        (&VirtualSerial_CDC_Interface, &USBSerialStream);

    touch_init();

    pwm16_rgb_init();
    
    /** enable interrupts*/
    sei();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void){
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void){
}

void EVENT_USB_Device_ConfigurationChanged(void){
    CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void){
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

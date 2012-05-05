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
    uint16_t sample;
    //char sBuffer[STRBUFLEN]; //!< character buffer for usb serial

    DDRB |= (1<<PB5) | (1<<PB6);
    DDRC |= (1<<PC6);

    //initialize
    initialize();

    i = 0;
    //main loop
    while (1)
    {
        i++;
        if (i>10000){
            if(USB_DeviceState == DEVICE_STATE_Configured){
                sample = touch_measure(1);
                fprintf(&USBSerialStream,"value: %u\t average: nothing!\r\n",sample);
            }
            i=0;
            PORTE ^= (1<<PE2);
            PORTB ^= (1<<PB5) | (1<<PB6);
            PORTC ^= (1<<PC6);
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

/*
uint16_t moving_average(uint16_t new){
    static uint16_t samples[16];
    static uint8_t i = 0;

    uint8_t j = 0;
    uint32_t retval = 0;
 
    samples[i++] = new;
    i %= 16;

    for(j=0 ; j<16 ; j++)
        retval += samples[j];

    return retval/16;

}
*/


/** Initializes all of the hardware. */
void initialize(void){

    CPU_PRESCALE(0x00);

    //LED
    DDRE |= (1<<PE2);

    /** LUFA USB related inits */
	USB_Init();
	CDC_Device_CreateBlockingStream
        (&VirtualSerial_CDC_Interface, &USBSerialStream);

    touch_init();
    
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

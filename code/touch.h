#ifndef _TOUCH_H_
#define _TOUCH_H_

/** Holds information related to a single touch channel */
typedef struct {
    volatile uint8_t *port;    //PORTx register for pin
    volatile uint8_t portmask; //mask for the bit in port
    volatile uint8_t mux;      //ADMUX value for the channel
    uint16_t min;
    uint16_t max;
} touch_channel_t;

/**Initializing the touch sensing */
void touch_init(void);

/**Doing a measurement */
uint16_t touch_measure(touch_channel_t *channel);


#endif

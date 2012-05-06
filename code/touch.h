#ifndef _TOUCH_H_
#define _TOUCH_H_

typedef struct {
    volatile uint8_t *port;
    volatile uint8_t portmask;
    volatile uint8_t mux;
    uint16_t min;
    uint16_t max;
} touch_channel_t;

void touch_init(void);

uint16_t touch_measure(touch_channel_t *channel);


#endif

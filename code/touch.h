#ifndef _TOUCH_H_
#define _TOUCH_H_

void touch_init(void);

uint16_t touch_measure(uint8_t channel);

typedef struct {
    uint8_t mux;
    uint8_t port;
    uint8_t ddr;
    uint8_t bit;
} touch_channel_t;

#endif

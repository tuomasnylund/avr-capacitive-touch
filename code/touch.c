//standard libs
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "touch.h"


static inline void adc_channel(uint8_t channel){
    ADMUX &= ~(0b11111);
    ADMUX |=   0b11111 & channel;
}

static inline uint16_t adc_get(void){
    ADCSRA |= (1<<ADSC);
    while(!(ADCSRA & (1<<ADIF)));
    ADCSRA |= (1<<ADIF);
    return ADC;
}

void touch_init(void){
    ADMUX  |= (1<<REFS0); //reference AVCC (3.3v)

    ADCSRA |= (1<<ADPS2)|(1<<ADPS1); //clockiv 64
    //final clock 8MHz/64 = 125kHz
    
    ADCSRA |= (1<<ADEN); //enable ADC

    DDRF &= ~(1<<PF6);
}


uint16_t touch_measure(touch_channel_t *channel){
    uint8_t i;
    uint16_t retval;

    retval = 0;

    for (i=0 ; i<4 ; i++){
        *(channel->port) |= channel->portmask;
        //PORTF  |= (1<<PF6);
        _delay_ms(1);
        //PORTF &= ~(1<<PF6);
        *(channel->port) &= ~(channel->portmask);

        adc_channel(0b11111); //set ADC mux to ground;
        adc_get();

        //adc_channel(6);
        adc_channel(channel->mux);
        retval +=  adc_get();
    }
    retval /= 4;

    if (retval < channel->min)
        channel->min = retval;

    if (retval > channel->max)
        channel->max = retval;

    retval = (retval - channel->min) * (0x7FFF/(channel->max - channel->min));

    return retval;
}

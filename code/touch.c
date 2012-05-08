//standard libs
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "touch.h"

/** For setting the ADC channel */
static inline void adc_channel(uint8_t channel){
    ADMUX &= ~(0b11111);
    ADMUX |=   0b11111 & channel;
}

/** For doing an ADC measurement */
static inline uint16_t adc_get(void){
    ADCSRA |= (1<<ADSC);
    while(!(ADCSRA & (1<<ADIF)));
    ADCSRA |= (1<<ADIF);
    return ADC;
}

void touch_init(void){
    ADMUX  |= (1<<REFS0); //reference AVCC (5v)

    ADCSRA |= (1<<ADPS2)|(1<<ADPS1); //clockiv 64
    //final clock 8MHz/64 = 125kHz
    
    ADCSRA |= (1<<ADEN); //enable ADC
}


uint16_t touch_measure(touch_channel_t *channel){
    uint8_t i;
    uint16_t retval;

    retval = 0;

    //Do four measurements and average, just to smooth things out
    for (i=0 ; i<4 ; i++){
        *(channel->port) |= channel->portmask;    // set pullup on
        _delay_ms(1);                             // wait (could probably be shorter)
        *(channel->port) &= ~(channel->portmask); // set pullup off

        adc_channel(0b11111); //set ADC mux to ground;
        adc_get();            //do a measurement (to discharge the sampling cap)

        adc_channel(channel->mux); //set mux to right channel
        retval +=  adc_get();
    }
    retval /= 4;

    if (retval < channel->min)
        return 0;

    if (retval > channel->max)
        return 0x7FFF;

    /*
    if (retval < channel->min)
        channel->min = retval;

    if (retval > channel->max)
        channel->max = retval;
        */

    retval = (retval - channel->min) * (0x7FFF/(channel->max - channel->min));

    return retval;
}

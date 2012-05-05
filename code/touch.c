//standard libs
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "touch.h"

/*
static touch_channel_t btn1 = {
    .mux = ADC5;
    .port = PORTF;
    .ddr = DDRF;
    .bit = PF7;
};
*/

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


uint16_t touch_measure(uint8_t channel){
    uint8_t i;
    uint16_t retval;

    retval = 0;



    adc_channel(7);
    for (i=0 ; i<8 ; i++){
        PORTF  |= (1<<PF6);
        _delay_ms(1);
        PORTF &= ~(1<<PF6);
        DIDR0 |= (1<<ADC6D);

        adc_channel(0b11111); //set ADC mux to ground;
        adc_get();

        adc_channel(6);
        retval +=  adc_get();
    }

    DIDR0 &= ~(1<<ADC6D);

    return retval;
}

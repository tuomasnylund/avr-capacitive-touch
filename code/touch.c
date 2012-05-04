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
    while(!(ADCSRA & ADIF));
    ADCSRA |= (1<<ADIF);
    return ADC;
}

void touch_init(void){
    ADMUX |= (1<<REFS0); //reference AVCC (3.3v)
    ADCSRA |= (1<<ADEN); //enable ADC
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1); //clockiv 64
    //final clock 8MHz/64 = 125kHz
}


uint16_t touch_measure(uint8_t channel){
    adc_channel(0xFF); //set ADC mux to ground;
    PORTF  |= (1<<PF7);
    ADCSRA |= (1<<ADSC);
    adc_get();


    PORTF &= ~(1<<PF7);
    adc_channel(5);
    return adc_get();
}

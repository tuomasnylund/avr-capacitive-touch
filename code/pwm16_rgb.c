/*
* File: RGB LED control for AVR
* Author: Tuomas Nylund (tuomas.nylund@gmail.com)
* Website: http://tuomasnylund.fi
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "pwm16_rgb.h"

//target values
uint16_t pwm16_red;
uint16_t pwm16_green;
uint16_t pwm16_blue;

//smoothness
uint16_t smooth_step;

void pwm16_rgb_init(void){
    /** Timer1 output compre registers need to be 
     *  set to output for the timer to work 
     */
    PWM_RED_DDR   |= (1<<PWM_RED_BIT);
    PWM_GREEN_DDR |= (1<<PWM_GREEN_BIT);
    PWM_BLUE_DDR  |= (1<<PWM_BLUE_BIT);

    /** Set output comare to set on match, clear at top */
    TCCR1A |= (1<<COM1A1);
    TCCR1A |= (1<<COM1B1);
    TCCR3A |= (1<<COM3A1);

    /** Set to Fast PWM, TOP at ICR1 */
    TCCR1B |= (1<<WGM13);
    TCCR1B |= (1<<WGM12);
    TCCR1A |= (1<<WGM11);
    
    TCCR3B |= (1<<WGM33);
    TCCR3B |= (1<<WGM32);
    TCCR3A |= (1<<WGM31);
    
    /** No clock prescaling */
    TCCR1B |= (1<<CS10);
    TCCR3B |= (1<<CS30);

    /** Enable overflow interrupt */
    TIMSK1 |= (1<<TOIE1);
    TIMSK3 |= (1<<TOIE3);

    /** Init all of the registers */
    ICR1  = 0xFFFF;
    ICR3  = 0xFFFF;
    OCR1A = 0;
    OCR1B = 0;
    OCR3A = 0;

    /** Init all of the default values */
    pwm16_red   = 0;
    pwm16_green = 0;
    pwm16_blue  = 0;

    smooth_step = 500;
}

void pwm16_set_values(uint16_t red, uint16_t green, uint16_t blue){
    pwm16_red   = red;
    pwm16_green = green;
    pwm16_blue  = blue;
}

void pwm16_set_smooth_step(uint16_t smoothness){
    smooth_step = smoothness;
}

uint16_t pwm16_smooth(uint16_t oldval, uint16_t target){
    uint16_t difference;

    if (oldval < target){
        difference = target - oldval;
        if(difference > smooth_step)
            oldval += smooth_step;
        else
            oldval = target;
    }

    else if (oldval > target){
        difference = oldval - target;
        if(difference > smooth_step)
            oldval -= smooth_step;
        else
            oldval = target;
    }

    return oldval;
}

ISR(TIMER1_OVF_vect){
    OC_RED   = pwm16_smooth(OC_RED,   pwm16_red);
    OC_GREEN = pwm16_smooth(OC_GREEN, pwm16_green);
}

ISR(TIMER3_OVF_vect){
    OC_BLUE  = pwm16_smooth(OC_BLUE,  pwm16_blue);
}

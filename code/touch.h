/*
* File: capacitive touch sense for AVR
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

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

#ifndef _PWM16_RGB_H_
#define _PWM16_RGB_H_

//* OC1.B */
#define PWM_RED_DDR   DDRB
#define PWM_RED_BIT   PB6
#define OC_RED        OCR1B

//* OC3.A */
#define PWM_BLUE_DDR  DDRC
#define PWM_BLUE_BIT  PC6
#define OC_BLUE       OCR3A

//* OC1.A */
#define PWM_GREEN_DDR DDRB
#define PWM_GREEN_BIT PB5
#define OC_GREEN      OCR1A


void pwm16_rgb_init(void);

void pwm16_set_values(uint16_t red, uint16_t green, uint16_t blue);
void pwm15_set_smooth_step(uint16_t smoothness);

#endif

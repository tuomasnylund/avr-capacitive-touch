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

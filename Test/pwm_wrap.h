/*
 * pwm_wrap.h
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#ifndef PWM_WRAP_H_
#define PWM_WRAP_H_

#include <stdint.h>

#ifndef PWM_REQ
#define PWM_REQ 0
#endif

#if PWM_REQ == 1
    #error "Analog writes on the TODO list"
#endif


void EnablePWM();
void WritePWM(uint32_t ui32Port, uint8_t ui8Pins, uint16_t value);

#endif /* PWM_WRAP_H_ */

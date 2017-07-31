/*
 * pwm_wrap.h
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#ifndef PWM_WRAP_H_
#define PWM_WRAP_H_

#include <stdint.h>
#include "pin_wrap.h"

//Transmission to motors/linear actuators/other things
const uint16_t pwm_limit = 1000;

void EnablePWM();
void WritePWM(uint32_t ui32Port, uint8_t ui8Pins, uint16_t value);

#endif /* PWM_WRAP_H_ */

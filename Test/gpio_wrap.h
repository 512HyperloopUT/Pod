/*
 * gpio_wrap.h
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#ifndef GPIO_WRAP_H_
#define GPIO_WRAP_H_

#include "pin_wrap.h"

#include <stdint.h>

/*****************
Peripheral enabling
*****************/
void MassPeriphInit(const uint32_t* ports, uint8_t len);

/*****************
Pin GPIO Init Macros and Functions
*****************/
#define PIN_IN 1
#define PIN_OUT 0
#define PTO GPIOPinTypeGPIOOutput
#define PTI GPIOPinTypeGPIOInput
#define GPIOPinInit(port, pin, type) type ? PTI(port, pin) : PTO(port, pin)

void GPIOMassInit(const uint32_t *ports, const uint8_t *pins, uint8_t len, uint8_t direction);

#endif /* GPIO_WRAP_H_ */

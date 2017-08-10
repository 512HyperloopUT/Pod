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

void GPIOPinInit(const uint32_t port, const uint8_t pin, const uint8_t type);

void GPIOMassInit(const uint32_t *ports, const uint8_t *pins, uint8_t len, uint8_t direction);

/*****************
Digital Pin IO
*****************/
#define DigiWritePin GPIOPinWrite
#define DigiReadPin GPIOPinRead

/* Only up to 64 bits, read starts reading to bit 0 */
uint64_t MassRead(const uint32_t *ports, const uint8_t *pins, uint8_t len);
/* Only up to 64 bits, write starting with bit 0 */
void MassWrite(const uint32_t *ports, const uint8_t *pins, uint8_t len, uint64_t data);

#endif /* GPIO_WRAP_H_ */

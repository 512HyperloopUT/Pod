/*
 * gpio_wrap.c
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#include "pin_wrap.h"
#include "gpio_wrap.h"

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "utils/uartstdio.h"

/*****************
Peripheral enabling
*****************/
void MassPeriphInit(const uint32_t* ports, uint8_t len) {
    int i;
    for (i = 0; i < len; ++i) {
        PeriphEnable(ports[i]);
        while (!PeriphReady(ports[i]));
    }
}
/*****************
Pin GPIO Init Macros and Functions
*****************/
void GPIOPinInit(const uint32_t port, const uint8_t pin, const uint8_t type) {
	if (type) {
		PTI(port, pin);
	} else {
		PTO(port, pin);
	}
}

void GPIOMassInit(const uint32_t *ports, const uint8_t *pins, uint8_t len, uint8_t direction) {
    int i;
    for (i = 0; i < len; ++i) {
        GPIOPinInit(ports[i], pins[i], direction);
    }
}
/*****************
Digital Pin IO
*****************/
/* Only up to 64 bits, read starts reading to bit 0 */
uint64_t MassRead(const uint32_t *ports, const uint8_t *pins, uint8_t len) {
    int i;
    uint64_t val = 0;
    for (i = 0; i < len; ++i) {
        int justnow = (DigiReadPin(ports[i], pins[i]) ? 1 : 0);
        val |= justnow << i;
    }
    return val;
}

/* Only up to 64 bits, write starting with bit 0 */
void MassWrite(const uint32_t *ports, const uint8_t *pins, uint8_t len, uint64_t data) {
    int i;
    for (i = 0; i < len; ++i) {
        DigiWritePin(ports[i], pins[i], ((data >> i) & 1) ? -1 : 0);
    }
}


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
        UARTprintf("Began init: %d\n", i);
        PeriphEnable(ports[i]);
        while (!PeriphReady(ports[i]));
        UARTprintf("Completed init: %d\n", i);
    }
}
/*****************
Pin GPIO Init Macros and Functions
*****************/
#define PIN_IN 1
#define PIN_OUT 0
#define PTO GPIOPinTypeGPIOOutput
#define PTI GPIOPinTypeGPIOInput
#define GPIOPinInit(port, pin, type) type ? PTI(port, pin) : PTO(port, pin)

void GPIOMassInit(const uint32_t *ports, const uint8_t *pins, uint8_t len, uint8_t direction) {
    int i;
    for (i = 0; i < len; ++i) {
        UARTprintf("Began init: %d, port: %d, pin: %d\n", i, ports[i], pins[i]);
        GPIOPinInit(ports[i], pins[i], direction);
        UARTprintf("Finished init: %d, port: %d, pin: %d\n", i, ports[i], pins[i]);
    }
}

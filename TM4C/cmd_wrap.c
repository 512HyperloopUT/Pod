/*
 * cmd_wrap.c
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#include "cmd_wrap.h"

/* C libraries */
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
/* Other, TM4C libraries */
/* hardware specific */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
/* Driver */
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
/* UART */
#include "utils/uartstdio.h"

#include "pin_wrap.h"
#include "mapping_constants.h"
#include "gpio_wrap.h"
#include "adc_wrap.h"
#include "pwm_wrap.h"

#include "utils/UARTstdio.h"

/*****************
Instruction management
*****************/
/* Past and current values */
uint8_t valid = 0;
uint8_t cmd = 0;
uint8_t new = 0;

/* Fetch instructions */
void ReadCmd(void) {
    /* Read cmd & idata */
    if (valid = DigiReadPin(pistt_ports[0], pistt_pins[0])) { /* Valid command */
        cmd = (uint8_t) MassRead(cmd_ports, cmd_pins, cmd_width);
    } else {
        DigiWritePin(tistt_ports[0], tistt_pins[0], 0);
	}
}

/* Execute instructions */

void ExecCmd(void) {
    if (!valid) {
		new = 1;
    } else if (new) { /* Do cmd if is new */
        new = 0;
		if (cmd != 31) {
			//TODO Read the selected sensor
			uint32_t adc_value = MapADCRead(cmd);
			UARTprintf("%d\n", adc_value);
		}
        /* Command has completed */
        DigiWritePin(tistt_ports[0], tistt_pins[0], -1);
		if (cmd == 31) {
			SysCtlReset();
		}
    }
}

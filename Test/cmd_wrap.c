/*
 * cmd_wrap.c
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#include "cmd_wrap.h"

/* C libraries */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
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
        int last_cmd = cmd;
        cmd = (uint8_t) MassRead(cmd_ports, cmd_pins, cmd_width);
        if (cmd != last_cmd) {
            new = 1;
        }
		UARTprintf("Command read:\n\tcmd: %ud\n", cmd);
    } else {
		UARTprintf("Command not yet valid.\n");
	}
}

/* Execute instructions */

void ExecCmd(void) {
    if (!valid) {
		new = 1;
        UARTprintf("Reset command.\n");
        DigiWritePin(tistt_ports[0], tistt_pins[0], 0);
    } else if (new || !cmd) { /* Do cmd if is new */
        new = 0;
        UARTprintf("Executing actual command.\n\tReading dummy sensor %d.\n", cmd);
		//TODO Read the selected sensor
		int adc_value = MapADCRead(cmd);
        /* Command has completed */
        DigiWritePin(tistt_ports[0], tistt_pins[0], -1);
		UARTprintf("Command executed.");
    }
}

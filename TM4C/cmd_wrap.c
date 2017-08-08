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
		UARTprintf("Command read: %ud\n", cmd);
    } else {
		UARTprintf("Command not valid.\n");
	}
}

/* Execute instructions */

void ExecCmd(void) {
    if (!valid) {
		new = 1;
        UARTprintf("Reset command.\n");
        DigiWritePin(tistt_ports[0], tistt_pins[0], 0);
		
			uint32_t adc_value = 1023;
			UARTprintf("\n%c%c%c%c%c%c\n", 242, (char) ((adc_value >> 24) & 0xff), (char) ((adc_value >> 16) & 0xff), (char) ((adc_value >> 8) & 0xff), (char) (adc_value & 0xff), 242);
    } else if (new) { /* Do cmd if is new */
        new = 0;
        UARTprintf("Executing actual command.\n");
		if (cmd != 31) {
			UARTprintf("\tReading dummy sensor %d.\n", cmd);
			//TODO Read the selected sensor
			uint32_t adc_value = MapADCRead(cmd);
			UARTprintf("\n%c%c%c%c%c%c\n", 242, (char) ((adc_value >> 24) & 0xff), (char) ((adc_value >> 16) & 0xff), (char) ((adc_value >> 8) & 0xff), (char) (adc_value & 0xff), 242);
		} else {
			UARTprintf("\tTriggering software reset %d.\n", cmd);
		}
        /* Command has completed */
        DigiWritePin(tistt_ports[0], tistt_pins[0], -1);
		UARTprintf("Command executed.\n");
		if (cmd == 31) {
			SysCtlReset();
			UARTprintf("Reset did not work. Please contact Benjamin Xu with a knife in hand. Just kidding.\n");
		}
    }
}

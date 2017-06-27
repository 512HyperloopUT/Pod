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

#include "mapping_constants.h"
#include "gpio_wrap.h"
#include "adc_wrap.h"
#include "pwm_wrap.h"


/*****************
Instruction management
*****************/
/* Past and current values */
uint8_t cmd = 0;
uint16_t idata = 0;

uint16_t curr_odata = 0;
uint8_t curr_pistt = 0;

/* Fetch instructions */
void ReadCmd(void) {
    /* Read cmd & idata */
    if (DigiReadPin(pistt_ports[0], pistt_pins[0])) { /* Valid command */
        cmd = (uint8_t) MassRead(cmd_ports, cmd_pins, cmd_width);
        idata = (uint16_t) MassRead(idata_ports, idata_pins, idata_width);
    } else {
        cmd = 0;
        idata = 0;
    }
    UARTprintf("Command read:\n\tcmd: %ud\n\tidata: %ud\n", cmd, curr_idata);
}

/* Execute instructions */
uint64_t last_exec = 0;
uint16_t exec_sep = 25; /* Maybe these should be shorter? Or longer? */
uint32_t adcbuffer[8]; /* For reading from the ADC */

void ExecCmd(void) {
    if (!cmd) {
        DigiWritePin(tistt_ports[0], tistt_pins[0], 0);
    } else { /* Do cmd */
        if (cmd == 1) { //Read or special function
            if (idata > 0xff) { /* idata >= 0 && idata < 0x200*/
                //TODO Read the selected sensor
                UARTprintf("Reading dummy sensor %d.", idata);
            } else { /* idata >= 0x200 && idata < 0x400*/
                //TODO Do a preset action
                UARTprintf("No preset actions. Invalid command.");
            }
        } else { /* Write */
            //TODO write target as 11, or some other out of bounds value
            MassWrite(tx_targ_ports, tx_targ_pins, tx_targ_width, 0xff /* Translate? */);
            //TODO write PWM
            MassWrite(tx_targ_ports, tx_targ_pins, tx_targ_width, cmd /* Translate? */);
            UARTprintf("Writing dummy value %d to dummy motor %d.", idata, cmd);
        }
        /* Command has completed */
        DigiWritePin(tistt_ports[0], tistt_pins[0], -1);
    }
}

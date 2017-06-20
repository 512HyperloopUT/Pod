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
uint8_t curr_cmd = 0;
uint8_t last_cmd = 0;
uint16_t curr_idata = 0;
uint16_t last_idata = 0;
uint16_t curr_odata = 0;
uint8_t curr_stt = 0;

/* Fetch instructions */
void ReadCmd(void) {
    /* Read cmd */
    last_cmd = curr_cmd;
    curr_cmd = (uint8_t) MassRead(cmd_ports, cmd_pins, cmd_width);
    /* Read idata */
    last_idata = curr_idata;
    curr_idata = (uint16_t) MassRead(idata_ports, idata_pins, idata_width);

    UARTprintf("Command read:\n\tcmd: %ud\n\tidata: %ud\n", curr_cmd, curr_idata);
}

/* Specialized function */
void PresetFunc(uint16_t cmd) {
    switch(cmd) {
        default:
            UARTprintf("Unrecognized command.");
            while(1);
    }
}

#if PWM_REQ == 1 || ADC_REQ == 1
uint32_t DecodeIData(uint16_t port, uint8_t mode) {
    uint8_t cnt;
    uint8_t cmtx;
    if (mode == 0) {
        cnt = rcnt;
        cmtx = rcmtx;
    } else if (mode == 1) {
        cnt = wcnt;
        cmtx = wcmtx;
    } else {
        while(1);
    }
    int i;
    for (i = 0; i < rcnt; i++) {
        if (port%cnt == cmtx[i][0]) {
            return cmtx[i][1];
        }
    }
    UARTprintf("Error %s port %u. Port not found.", mode?"reading from":"writing to", conv);
    return 0;
}
#endif
/* Execute instructions */
uint64_t last_exec = 0;
uint16_t exec_sep = 25;
uint32_t adcbuffer[8];
void ExecCmd(void) {
#if PI_ADVANCED == 1
    if (
            //Changed command
            (curr_cmd != last_cmd || curr_idata != last_idata)
            //OR command redo time reached, or is first and is not write
            || (curr_time - last_exec >= exec_sep || exec_beat == 0)
    ) {
        /* Set time data */
        last_exec = curr_time;
        switch (curr_cmd) {
        case 0:
        case 1: /* Toggling signal */
            curr_stt = !curr_cmd;
            DigiWritePin(stt_ports[0], stt_pins[0], curr_stt);
            if (curr_idata < 0x800) { /* Read some signal */
            #if ADC_REQ == 1
                ReadADC(ADC0_BASE, 0, &adcbuffer);
                curr_odata = adcbuffer[DecodeIData(curr_idata, 1)];
            #else
                while(1);
            #endif
            } else { /* Preset instruction */
            #if CSTM_FN == 1
                PresetFunc(curr_cmd, curr_idata);
            #else
                while(1);
            #endif
            }
            curr_stt = curr_cmd;
            break;
        default: /* Transmit some signal */
        #if PWM_REQ == 1
            uint8_t write = DecodeIData(curr_cmd, 0);
            MassWrite(tx_targ_ports[write], tx_targ_pins[write], tx_targ_width, write);
            WritePWM(tx_ports[0], tx_pins[0], curr_idata);
        #else
            UARTprintf("We broke. PWM is not allowed.\n", curr_cmd, curr_idata);
            while(1);
        #endif
        }
        /* Read necessary data */
        /* Write valp */
        MassWrite(odata_ports, odata_pins, odata_width, curr_odata);
        /* Write s */
        DigiWritePin(stt_ports[0], stt_pins[0], curr_stt);
    }
#elif PI_SIMPLE == 1
    UARTprintf("Command read:\n\tcmd: %ud\n", curr_cmd, curr_idata);
    MassWrite(stt_ports, stt_pins, stt_width, !curr_cmd);
#else
    #error "No Pi communication protocol established"
#endif
}

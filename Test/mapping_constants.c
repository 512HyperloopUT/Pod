/*
 * mapping_constants.c
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

/* Base header */
#include "pin_wrap.h"

/* C libraries */
#include <stdint.h>
#include <stdbool.h>

/* Other, TM4C libraries */
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

/*****************
Configure communication with the Raspberry Pi, pin mappings and method.

Listed from listed significant to most significant bit
*****************/

//State of cmd execution
uint8_t stt_width = 1;
uint32_t stt_ports[] = {PORT(A)};
uint8_t stt_pins[] = {PIN(7)};
//Data retrieved
uint8_t odata_width = 12;
uint32_t odata_ports[] = {
    PORT(C), PORT(C), PORT(N), PORT(F),
    PORT(F), PORT(G), PORT(G), PORT(H),
    PORT(H), PORT(H), PORT(H), PORT(H)
};
uint8_t odata_pins[] = {
    PIN(6), PIN(7), PIN(3), PIN(2),
    PIN(3), PIN(0), PIN(1), PIN(0),
    PIN(1), PIN(2), PIN(3), PIN(4)
};
//Command to execute, 0&1 are the reading toggle values, and 2-15 are the write commands
uint8_t cmd_width = 4;
uint32_t cmd_ports[] = {
    PORT(K), PORT(K), PORT(K), PORT(L)
};
uint8_t cmd_pins[] = {
    PIN(5), PIN(6), PIN(7), PIN(0)
};
//Data for supplementing the command,
//  sensor number (0-0x7ff) or preprogrammed action (0x800-0xfff) when reading,
//  intensity when writing
uint8_t idata_width = 12;
uint32_t idata_ports[] = {
    PORT(L), PORT(L), PORT(L), PORT(L),
    PORT(L), PORT(M), PORT(M), PORT(M),
    PORT(M), PORT(M), PORT(M), PORT(N)
};
uint8_t idata_pins[] = {
    PIN(1), PIN(2), PIN(3), PIN(4),
    PIN(5), PIN(0), PIN(1), PIN(2),
    PIN(3), PIN(6), PIN(7), PIN(2)
};
//Used GPIO ports
uint8_t picomm_port_count = 11;
uint32_t picomm_ports[] = {
    GPIOPORT(A), GPIOPORT(C), GPIOPORT(F), GPIOPORT(G),
    GPIOPORT(H), GPIOPORT(K), GPIOPORT(L), GPIOPORT(M),
    GPIOPORT(N)
};
//Transmission to motors/linear actuators/other things
uint8_t tx_width = 1;
uint32_t tx_ports[] = {
    //PORT(F)
};
uint8_t tx_pins[] = {
    //PIN(1)
};
//Transmission target selection
uint8_t tx_targ_width = 4;
uint32_t tx_targ_ports[] = {
    //PORT(P), PORT(P), PORT(P), PORT(Q)
};
uint8_t tx_targ_pins[] = {
    //PIN(2), PIN(3), PIN(5), PIN(1)
};
uint8_t tx_output_ports_cnt = 2;
uint32_t tx_output_ports[] = {
    //PORT(P), PORT(Q)
};
//Reception from sensors/other things
uint8_t rx_width = 6;

#if ADC_REQ == 1
uint8_t rcnt = 0;
uint8_t rcmtx[][] = {
    // Some format of input to pin
    {}
};
#endif
#if PWM_REQ == 1
uint8_t wcnt = 0;
uint8_t wc2mtx[] = {
    //Some value the end target will understand
};
uint8_t wcmtx[][] = {
    //Some format of input to pin
    {}
}
#endif


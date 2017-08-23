/*
 * mapping_constants.c
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

/* Base header */
#include "mapping_constants.h"
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
const uint8_t tistt_width = 1;
const uint32_t tistt_ports[] = {PORT(L)};
const uint8_t tistt_pins[] = {PIN(2)};
//Command to execute, 0&1 are the reading toggle values, and 2-15 are the write commands
const uint8_t cmd_width = 5;
const uint32_t cmd_ports[] = {
    PORT(F), PORT(H), PORT(H), PORT(K),
    PORT(K)
};
const uint8_t cmd_pins[] = {
    PIN(3), PIN(0), PIN(1), PIN(6),
    PIN(7)
};
//State of cmd writing
const uint8_t pistt_width = 1;
const uint32_t pistt_ports[] = {PORT(L)};
const uint8_t pistt_pins[] = {PIN(3)};

//Port for write cmd
const uint8_t cmd_type_width = 1;
const uint32_t cmd_type_ports[] = {PORT(L)};
const uint8_t cmd_type_pins[] = {PIN(2)};

//Port for write cmd direction
const uint8_t cmd_dir_width = 2;
const uint32_t cmd_dir_ports[] = {PORT(F), PORT(F)};
const uint8_t cmd_dir_pins[] = {PIN(1), PIN(2)};

//Used GPIO ports
const uint8_t picomm_port_count = 4;
const uint32_t picomm_ports[] = {
    GPIOPORT(F), GPIOPORT(H), GPIOPORT(K), GPIOPORT(L)
};


//Write pins
const uint8_t actu_write_count = 0;
const uint32_t actu_forw_write_ports[] = {};
const uint8_t actu_forw_write_pins[] = {};
const uint32_t actu_back_write_ports[] = {};
const uint8_t actu_back_write_pins[] = {};
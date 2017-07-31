/*
 * mapping_constants.h
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#ifndef MAPPING_CONSTANTS_H_
#define MAPPING_CONSTANTS_H_

#include <stdint.h>

#include "pin_wrap.h"

/*****************
Listed from least significant to most significant bit
*****************/

//State of cmd execution
extern const uint8_t tistt_width;
extern const uint32_t tistt_ports[];
extern const uint8_t tistt_pins[];

//State of cmd provided
extern const uint8_t pistt_width;
extern const uint32_t pistt_ports[];
extern const uint8_t pistt_pins[];

//Port to read
extern const uint8_t cmd_width;
extern const uint32_t cmd_ports[];
extern const uint8_t cmd_pins[];

//Used GPIO ports for Pi-TM4C comms
extern const uint8_t picomm_port_count;
extern const uint32_t picomm_ports[];

#endif /* MAPPING_CONSTANTS_H_ */

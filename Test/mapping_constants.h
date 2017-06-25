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
extern const uint8_t stt_width;
extern const uint32_t stt_ports[];
extern const uint8_t stt_pins[];

//Data retrieved
extern const uint8_t odata_width;
extern const uint32_t odata_ports[];
extern const uint8_t odata_pins[];

//Command to execute, 0&1 are the reading toggle values, and 2-15 are the write commands
extern const uint8_t cmd_width;
extern const uint32_t cmd_ports[];
extern const uint8_t cmd_pins[];

//Data for supplementing the command,
//  sensor number (0-0x7ff) or preprogrammed action (0x800-0xfff) when reading,
//  intensity when writing
extern const uint8_t idata_width;
extern const uint32_t idata_ports[];
extern const uint8_t idata_pins[];

//Used GPIO ports for Pi-TM4C comms
extern const uint8_t picomm_port_count;
extern const uint32_t picomm_ports[];

#if ADC_REQ == 1
//Reception from sensors/other things
extern const uint8_t rx_width;

extern const uint8_t rcnt;
extern const uint8_t rcmtx[][];
#endif
#if PWM_REQ == 1
extern const uint16_t pwm_limit;

//Transmission to motors/linear actuators/other things
extern const uint8_t tx_width;
extern const uint32_t tx_ports[];
extern const uint8_t tx_pins[];

//Transmission target selection
extern const uint8_t tx_targ_width;
extern const uint32_t tx_targ_ports[];
extern const uint8_t tx_targ_pins[];
extern const uint8_t tx_output_ports_cnt;
extern const uint32_t tx_output_ports[];
#endif


#endif /* MAPPING_CONSTANTS_H_ */

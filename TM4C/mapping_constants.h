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

//Port for write cmd
extern const uint8_t cmd_type_width;
extern const uint32_t cmd_type_ports[];
extern const uint8_t cmd_type_pins[];

//Port for write cmd direction
extern const uint8_t cmd_dir_width;
extern const uint32_t cmd_dir_ports[];
extern const uint8_t cmd_dir_pins[];

//Used GPIO ports for Pi-TM4C comms
extern const uint8_t picomm_port_count;
extern const uint32_t picomm_ports[];

//Digital read pins
extern const uint8_t digi_read_count;
extern const uint32_t digi_read_ports[];
extern const uint8_t digi_read_pins[];

//Write pins
extern const uint8_t actu_write_count;
extern const uint32_t actu_forw_write_ports[];
extern const uint8_t actu_forw_write_pins[];
extern const uint32_t actu_back_write_ports[];
extern const uint8_t actu_back_write_pins[];

#endif /* MAPPING_CONSTANTS_H_ */

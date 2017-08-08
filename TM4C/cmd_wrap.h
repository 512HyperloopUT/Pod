/*
 * cmd_wrap.h
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#ifndef CMD_WRAP_H_
#define CMD_WRAP_H_
/* C libraries */
#include <stdint.h>
#include <stdbool.h>

#include "pin_wrap.h"
#include "mapping_constants.h"

/*****************
Instruction management
*****************/
/* Fetch instructions */
void ReadCmd(void);

/* Execute instructions */
void ExecCmd();

#endif /* CMD_WRAP_H_ */

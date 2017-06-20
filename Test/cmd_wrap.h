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

#include "mapping_constants.h"

#ifndef PI_ADVANCED
    #ifdef PI_SIMPLE
        #if PI_SIMPLE == 0
            #define PI_ADVANCED 1
        #else
            #define PI_ADVANCED 0
        #endif
    #else
        #define PI_ADVANCED 0
    #endif
#endif

#ifndef PI_SIMPLE
    #if PI_ADVANCED == 0
        #define PI_SIMPLE 1
    #else
        #define PI_SIMPLE 0
    #endif
#endif

/*****************
Instruction management
*****************/
/* Fetch instructions */
void ReadCmd(void);
/* Specialized function */
void PresetFunc(uint16_t cmd);

#if PWM_REQ == 1 || ADC_REQ == 1
uint32_t DecodeIData(uint16_t port, uint8_t mode);
#endif

/* Execute instructions */
void ExecCmd(void);

#endif /* CMD_WRAP_H_ */

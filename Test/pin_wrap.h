/*
 * pin_wrap.h
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#ifndef PIN_WRAP_H_
#define PIN_WRAP_H_


#define PeriphEnable SysCtlPeripheralEnable
#define PeriphReady SysCtlPeripheralReady
/*****************
Pin Mapping Macros
*****************/
#define GPIOPORT(pin) SYSCTL_PERIPH_GPIO ## pin
#define PORT(port) GPIO_PORT ## port ## _BASE
#define PIN(pin) GPIO_PIN_ ## pin
#define PP(pin, port) PORT(pin), PIN(port)

#endif /* PIN_WRAP_H_ */

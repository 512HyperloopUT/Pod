/*
 * pwm_wrap.c
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#include "pwm_wrap.h"
#include "pin_wrap.h"

/* C libraries */
#include <stdint.h>
#include <stdbool.h>

/* Other, TM4C libraries */
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"


void EnablePWM() {
#if PWM_REQ == 1
    unsigned long ulPeriod, dutyCycle;
    ulPeriod = 1000;
    dutyCycle = 250;

    // Configure PB6 as T0CCP0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinConfigure(GPIO_PD0_T0CCP0);
    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);

    // Configure timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod -1);
    TimerMatchSet(TIMER0_BASE, TIMER_A, dutyCycle); // PWM
    TimerEnable(TIMER0_BASE, TIMER_A);
#endif
}

void WritePWM(uint32_t ui32Port, uint8_t ui8Pins, uint16_t value) {
#if PWM_REQ == 1
    TimerMatchSet(TIMER0_BASE, TIMER_A, value);
#endif
}

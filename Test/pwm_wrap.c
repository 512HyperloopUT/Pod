/*
 * pwm_wrap.c
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#include "pwm_wrap.h"

/* C libraries */
#include <stdint.h>
#include <stdbool.h>

/* Other, TM4C libraries */
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"


void EnablePWM() {
#if PWM_REQ == 1
    /* Analog outputs */
    #error "Analog output must be configured."
    //TODO
    PeriphEnable(SYSCTL_PERIPH_PWM0);
    while(!PeriphReady(SYSCTL_PERIPH_PWM0));
     // Configure the PWM generator for count down mode with immediate updates to the parameters.
     PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
     // Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20 microseconds. For a 20 MHz clock, this translates to 400 clock ticks.
     // Use this value to set the period.
     PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 400);
     // Start the timers in generator 0.
     PWMGenEnable(PWM0_BASE, PWM_GEN_0);
     // Enable the outputs.
     PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
#endif
}

void WritePWM(uint32_t ui32Port, uint8_t ui8Pins, uint16_t value) {
    //TODO
    // Set the pulse width of PWM0 for a 25% duty cycle.
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 100);
    // Set the pulse width of PWM1 for a 75% duty cycle.
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 300);
}

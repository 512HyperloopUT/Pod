/*
 * adc_wrap.c
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#include "adc_wrap.h"

/* C libraries */
#include <stdint.h>
#include <stdbool.h>

/* Other, TM4C libraries */
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"

#include "utils/uartstdio.h"


#define ADCCH(num) ADC_CTL_CH  ##  num
uint32_t channel_mapping[] = {
	ADCCH(0), ADCCH(1), ADCCH(2), ADCCH(3),
	ADCCH(4), ADCCH(5), ADCCH(6), ADCCH(7),
	ADCCH(8), ADCCH(9), ADCCH(10), ADCCH(11),
	ADCCH(12), ADCCH(13), ADCCH(14), ADCCH(15),
	ADCCH(16), ADCCH(17), ADCCH(18), ADCCH(19)
};

uint32_t MapADCRead(uint32_t val) {
	uint32_t buff = 0;
	ADCRead(channel_mapping[val], 0, &buff);
	
	return buff;
}

void ADCRead(uint32_t channel, bool masked, uint32_t* buffer) {
	//Reconfigure adc for target channel
    ADCSequenceDisable(ADC0_BASE, 3);
    //
    // Configure step 0 on sequence 3.  Sample channel, as provided, in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // information on the ADC sequences and steps, reference the datasheet.
    //
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, channel | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    //
    ADCSequenceEnable(ADC0_BASE, 3);


	//Do actual read
    ADCIntClear(ADC0_BASE, 3); //Clear interrupt flag
    ADCProcessorTrigger(ADC0_BASE, 3); //Trigger interrupt
    while(!ADCIntStatus(ADC0_BASE, 3, masked)); //Wait for sequencer
    ADCIntClear(ADC0_BASE, 3); //Clear interrupt flag
    ADCSequenceDataGet(ADC0_BASE, 3, buffer); //Fetch the values into a buffer
	
	return buffer;
}

void EnableADC() {
    //
    // The ADC0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //
    // For this example ADC0 is used with AIN0 on port E7.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.  GPIO port E needs to be enabled
    // so these pins can be used.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);

    //
    // Select the analog ADC function for these pins.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_7);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_6);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_5);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_4);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_5);
	GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4);
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);
	GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_0);
	GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_1);
	GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_2);
	GPIOPinTypeADC(GPIO_PORTK_BASE, GPIO_PIN_3);

	//
    // Since sample sequence 3 is now configured, it must be enabled.
    //
    ADCSequenceDisable(ADC0_BASE, 3);

    //
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 3.
    //
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    //
    ADCSequenceEnable(ADC0_BASE, 3);

}


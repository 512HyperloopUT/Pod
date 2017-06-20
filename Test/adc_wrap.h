/*
 * adc_wrap.h
 *
 *  Created on: Jun 20, 2017
 *      Author: benbe
 */

#ifndef ADC_WRAP_H_
#define ADC_WRAP_H_

#include "pin_wrap.h"

#ifndef ADC_REQ
#define ADC_REQ = 1
#endif

void ADCRead(uint32_t base, uint32_t seq_num, bool masked, uint32_t* buffer) {
    ADCIntClear(base, seq_num); //Clear interrupt flag
    ADCProcessorTrigger(base, seq_num); //Trigger interrupt
    while(!ADCIntStatus(base, seq_num, masked)); //Wait for sequencer
    ADCIntClear(base, seq_num); //Clear interrupt flag
    ADCSequenceDataGet(base, seq_num, buffer); //Fetch the values into a buffer
}

void EnableADC() {
#if ADC_REQ == 1
    /* ADC initialization*/
    //Enable Peripheral
    PeriphEnable(SYSCTL_PERIPH_ADC0);
    while(!PeriphReady(SYSCTL_PERIPH_ADC0));
    /* Disable the sequences that we will use, so we can configure */
    ADCSequenceDisable(ADC0_BASE, 0);
    /* ADC sequence configure, sequencer 0, triggered from processor */
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    /* Configure sampling for the steps. */
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH4);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH5 | ADC_CTL_IE | ADC_CTL_END);
    /* Enable interrupt and sequence for sequencer 0 */
    ADCIntEnable(ADC0_BASE, 0);
    ADCSequenceEnable(ADC0_BASE, 0);
    ADCIntClear(ADC0_BASE, 0);
    //And now we finally done for ADC. Next is PWM
#endif
}

#endif /* ADC_WRAP_H_ */

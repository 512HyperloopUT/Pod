/* C libraries */
#include <stdint.h>
#include <stdbool.h>
/* Other, TM4C libraries */
/* hardware specific */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
/* Driver */
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/adc.h"
/* Board driver */
#include "drivers/pinout.h"
/* UART */
#include "utils/uartstdio.h"

/*****************
The error routine that is called if the driver library encounters an error
*****************/
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line) {
}
#endif

/*****************
Compile time capability selection
*****************/
/* Analog read is needed */
#define ADC_REQ 0
/* Analog write is needed */
#define PWM_REQ 0
/* Only stt and cmd IO ports */
#define PI_SIMPLE 1
/* All 4 ports needed */
#define PI_ADVANCED 0
/* Custom functions needed */
#define CSTM_FN 0

/*****************
Clock variables
*****************/
/* System clock rate in Hz */
uint32_t clock_freq;
uint64_t curr_time = 0;

/*****************
Peripheral enabling
*****************/
#define GPIOPORT(pin) SYSCTL_PERIPH_GPIO ## pin
#define PeriphEnable SysCtlPeripheralEnable
#define PeriphReady SysCtlPeripheralReady
void MassPeriphInit(uint32_t* ports, uint8_t len) {
    int i;
    for (i = 0; i < len; ++i) {
        PeriphEnable(ports[i]);
        while (!PeriphReady(ports[i]));
    }
}
/*****************
Pin Mapping Macros
*****************/
#define PORT(port) GPIO_PORT ## port ## _BASE
#define PIN(pin) GPIO_PIN_ ## pin
#define PP(pin, port) PORT(pin), PIN(port)
/*****************
Pin GPIO Init Macros and Functions
*****************/
#define PIN_IN 1
#define PIN_OUT 0
#define PTO GPIOPinTypeGPIOOutput
#define PTI GPIOPinTypeGPIOInput
#define GPIOPinInit(port, pin, type) type ? PTI(port, pin) : PTO(port, pin)
void GPIOMassInit(uint32_t *ports, uint8_t *pins, uint8_t len, uint8_t direction) {
    int i;
    for (i = 0; i < len; ++i) {
        GPIOPinInit(ports[i], pins[i], direction);
    }
}
/*****************
Digital Pin IO
*****************/
#define DigiWritePin GPIOPinWrite
#define DigiReadPin GPIOPinRead
/* Only up to 64 bits, read starts reading to bit 0 */
uint64_t MassRead(uint32_t *ports, uint8_t *pins, uint8_t len) {
    int i;
    uint64_t val = 0;
    for (i = 0; i < len; ++i) {
        val |= (DigiReadPin(ports[i], pins[i]) ? 1 : 0) << i;
    }
    return val;
}
/* Only up to 64 bits, write starting with bit 0 */
void MassWrite(uint32_t *ports, uint8_t *pins, uint8_t len, uint64_t data) {
    int i;
    for (i = 0; i < len; ++i) {
        DigiWritePin(ports[i], pins[i], ((data >> i) & 1) ? -1 : 0);
    }
}

/*****************
Analog Pin IO
*****************/
#if PWM_REQ == 1
    #error "Analog writes on the TODO list"
#endif
void ADCRead(uint32_t base, uint32_t seq_num, bool masked, uint32_t* buffer) {
    ADCIntClear(base, seq_num); //Clear interrupt flag
    ADCProcessorTrigger(base, seq_num); //Trigger interrupt
    while(!ADCIntStatus(base, seq_num, masked)); //Wait for sequencer
    ADCIntClear(base, seq_num); //Clear interrupt flag
    ADCSequenceDataGet(base, seq_num, buffer); //Fetch the values into a buffer
}

/*****************
Configure communication with the Raspberry Pi, pin mappings and method.

Listed from listed significant to most significant bit
*****************/
//State of cmd execution
uint8_t stt_width = 1;
uint32_t stt_ports[] = {PORT(A)};
uint8_t stt_pins[] = {PIN(7)};
//Data retrieved
uint8_t odata_width = 12;
uint32_t odata_ports[] = {
    PORT(C), PORT(C), PORT(N), PORT(F),
    PORT(F), PORT(G), PORT(G), PORT(H),
    PORT(H), PORT(H), PORT(H), PORT(H)
};
uint8_t odata_pins[] = {
    PIN(6), PIN(7), PIN(3), PIN(2),
    PIN(3), PIN(0), PIN(1), PIN(0),
    PIN(1), PIN(2), PIN(3), PIN(4)
};
//Command to execute, 0&1 are the reading toggle values, and 2-15 are the write commands
uint8_t cmd_width = 4;
uint32_t cmd_ports[] = {
    PORT(K), PORT(K), PORT(K), PORT(L)
};
uint8_t cmd_pins[] = {
    PIN(5), PIN(6), PIN(7), PIN(0)
};
//Data for supplementing the command,
//  sensor number (0-0x7ff) or preprogrammed action (0x800-0xfff) when reading,
//  intensity when writing
uint8_t idata_width = 12;
uint32_t idata_ports[] = {
    PORT(L), PORT(L), PORT(L), PORT(L),
    PORT(L), PORT(M), PORT(M), PORT(M),
    PORT(M), PORT(M), PORT(M), PORT(N)
};
uint8_t idata_pins[] = {
    PIN(1), PIN(2), PIN(3), PIN(4),
    PIN(5), PIN(0), PIN(1), PIN(2),
    PIN(3), PIN(6), PIN(7), PIN(2)
};
//Used GPIO ports
uint8_t picomm_port_count = 11;
uint32_t picomm_ports[] = {
    GPIOPORT(A), GPIOPORT(C), GPIOPORT(F), GPIOPORT(G),
    GPIOPORT(H), GPIOPORT(K), GPIOPORT(L), GPIOPORT(M),
    GPIOPORT(N)
};
void ConfigurePiComm(void) {
    /* GPIO ports */
    MassPeriphInit(picomm_ports, picomm_port_count);
    /* cmd */
    GPIOMassInit(cmd_ports, cmd_pins, cmd_width, PIN_IN);
    /* idata */
    GPIOMassInit(idata_ports, idata_pins, idata_width, PIN_IN);
    /* stt */
    GPIOMassInit(stt_ports, stt_pins, stt_width, PIN_OUT);
    /* odata */
    GPIOMassInit(odata_ports, odata_pins, odata_width, PIN_OUT);
}
/*****************
Configure output
*****************/
//Transmission to motors/linear actuators/other things
uint8_t tx_width = 1;
uint32_t tx_ports[] = {
    //PORT(F)
};
uint8_t tx_pins[] = {
    //PIN(1)
};
//Transmission target selection
uint8_t tx_targ_width = 4;
uint32_t tx_targ_ports[] = {
    //PORT(P), PORT(P), PORT(P), PORT(Q)
};
uint8_t tx_targ_pins[] = {
    //PIN(2), PIN(3), PIN(5), PIN(1)
};
uint8_t tx_output_ports_cnt = 2;
uint32_t tx_output_ports[] = {
    //PORT(P), PORT(Q)
};
//Reception from sensors/other things
uint8_t rx_width = 6;
void ConfigureOuput(void) {
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
#if PWM_REQ == 1
    /* Analog outputs */
    #error "Analog output must be configured."
    /* Target output */
    MassPeriphInit(tx_output_ports, tx_output_ports_cnt);
    GPIOMassInit(tx_targ_ports, tx_targ_pins, tx_targ_width, PIN_OUT);
#endif
    
}
/*****************
Configure the UART and its pins.  This must be called before UARTprintf()
Configures COM port with 115200 8N1
*****************/
void ConfigureUART(void) {
    /* Enable UART0 */
    PeriphEnable(SYSCTL_PERIPH_UART0);
    while (!PeriphReady(SYSCTL_PERIPH_UART0));
    /* Enable the GPIO Peripheral used by the UART */
    PeriphEnable(SYSCTL_PERIPH_GPIOA);
    while(!PeriphReady(SYSCTL_PERIPH_GPIOA));
    /* Configure GPIO Pins for UART mode */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    /* Initialize the UART for console I/O */
    UARTStdioConfig(0, 115200, clock_freq);
    /* Stands for
    UARTConfigSetExpClk(UART0_BASE, clock_freq, 115200,
                            (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_WLEN_8));
    UARTEnable(UART0_BASE);
    */
}
/*****************
Configure heartbeat
*****************/
uint16_t heartbeat_sep = 250;
ConfigureHeartbeat() {
    PeriphEnable(GPIOPORT(N));
    while(!PeriphReady(GPIOPORT(N)));
    /* Enable the GPIO pins for the LED D1, or GPIO PN1 */
    GPIOPinInit(PORT(N), PIN(1), PIN_OUT);
}
/*****************
Configure SysTick for use with timing data
*****************/
void SysTick_IntHandler(void) { curr_time++; }
void ConfigureTiming(void) {
    /* Settle clock */
    uint32_t clock = SysCtlClockGet();
    /* Make a function available for handling the SysTick */
    SysTickIntRegister(SysTick_IntHandler);
    /* Equivalent Clock speed and SysTick */
    SysTickPeriodSet(clock_freq);
    /* Enable interrupts */
    IntMasterEnable();
    /* Enables SysTick interrupts */
    SysTickIntEnable();
    /* Enable the SysTick itself */
    SysTickEnable();
}
/*****************
General setup
*****************/
void Setup() {
    /* Run from the PLL at 120 MHz */
    clock_freq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                            SYSCTL_OSC_MAIN |
                                            SYSCTL_USE_PLL |
                                            SYSCTL_CFG_VCO_480),
                                            120 * 1000 * 1000);
    /* Initialize the communication pins */
    ConfigureUART(); //UART
    UARTprintf("Hello. We are now configuring.");
    ConfigureOuput(); //Output to pins
    ConfigurePiComm(); //Pi communications
    ConfigureTiming(); //Systick setup
    UARTprintf("Configuration complete. We are ready to begin.");
}

/*****************
Instruction management
*****************/
/* Past and current values */
uint8_t curr_cmd = 0;
uint8_t last_cmd = 0;
uint16_t curr_idata = 0;
uint16_t last_idata = 0;
uint16_t curr_odata = 0;
uint8_t curr_stt = 0;
/* Fetch instructions */
void ReadCmd(void) {
    /* Read cmd */
    last_cmd = curr_cmd;
    curr_cmd = (uint8_t) MassRead(cmd_ports, cmd_pins, cmd_width);
    /* Read idata */
    last_idata = curr_idata;
    curr_idata = (uint16_t) MassRead(idata_ports, idata_pins, idata_width);
}
/* Specialized function */
void PresetFunc(uint16_t cmd) {
    switch(cmd) {
        default:
            UARTprintf("Unrecognized command.");
            while(1);
    }
}
#if ADC_REQ == 1
uint8_t rcnt = 0;
uint8_t rcmtx[][] = {
    // Some format of input to pin
    {}
};
#endif
#if PWM_REQ == 1
uint8_t wcnt = 0;
uint8_t wc2mtx[] = {
    //Some value the end target will understand
};
uint8_t wcmtx[][] = {
    //Some format of input to pin
    {}
}
#endif
#if PWM_REQ == 1 || ADC_REQ == 1
uint32_t DecodeIData(uint16_t port, uint8_t mode) {
    uint8_t cnt;
    uint8_t cmtx;
    if (mode == 0) {
        cnt = rcnt;
        cmtx = rcmtx;
    } else if (mode == 1) {
        cnt = wcnt;
        cmtx = wcmtx;
    } else {
        while(1);
    }
    int i;
    for (i = 0; i < rcnt; i++) {
        if (port%cnt == cmtx[i][0]) {
            return cmtx[i][1];
        }
    }
    UARTprintf("Error %s port %u. Port not found.", mode?"reading from":"writing to", conv);
    return 0;
}
#endif
/* Execute instructions */
uint64_t last_exec = 0;
uint16_t exec_sep = 25;
uint32_t adcbuffer[8];
void ExecCmd(void) {
#if PI_ADVANCED == 1
    if (
            //Changed command
            (curr_cmd != last_cmd || curr_idata != last_idata)
            //Command redo time reached, or is first and is not write
            || (curr_time - last_exec >= exec_sep || exec_beat == 0)
    ) {
        /* Set time data */
        last_exec = curr_time;
        switch (curr_cmd) {
        case 0:
        case 1: /* Toggling signal */
            curr_stt = !curr_cmd;
            DigiWritePin(stt_ports[0], stt_pins[0], curr_stt);
            if (curr_idata < 0x800) { /* Read some signal */
            #if ADC_REQ == 1
                ReadADC(ADC0_BASE, 0, &adcbuffer);
                curr_odata = adcbuffer[DecodeIData(curr_idata, 1)];
            #else
                while(1);
            #endif
            } else { /* Preset instruction */
            #if CSTM_FN == 1
                PresetFunc(curr_cmd, curr_idata);
            #else
                while(1);
            #endif
            }
            curr_stt = curr_cmd;
            break;
        default: /* Transmit some signal */
        #if PWM_REQ == 1
            uint8_t write = DecodeIData(curr_cmd, 0);
            MassWrite(tx_targ_ports[write], tx_targ_pins[write], tx_targ_width, write);
            WritePWM(tx_ports[0], tx_pins[0], curr_idata);
        #else
            while(1);
        #endif
        }
        /* Read necessary data */
        /* Write valp */
        MassWrite(odata_ports, odata_pins, odata_width, curr_odata);
        /* Write s */
        DigiWritePin(stt_ports[0], stt_pins[0], curr_stt);
    }
#elif PI_SIMPLE == 1
    MassWrite(stt_ports, stt_pins, stt_width, !curr_cmd);
#else
    #error "No Pi communication protocol established"
#endif
}

/*****************
Heartbeat
*****************/
uint64_t last_beat = 0;
uint8_t beat = 0;
void Heartbeat(void) {
    if (
            //Elapsed time exceeds time of separation
            curr_time - last_beat >= heartbeat_sep ||
            //Is start
            last_beat == 0
    ) {
        /* Flip state and toggle led */
        beat = !beat;
        last_beat = curr_time;
        DigiWritePin(PORT(N), PIN(1), PIN(1));
    }
}

/*****************
Generic loop function and main function
*****************/
inline void Loop(void) {
    ReadCmd();
    ExecCmd();
    Heartbeat();
}

int main(void) {
    Setup();
    while (1) {
        Loop();
    }
}

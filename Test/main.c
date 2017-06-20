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

/* Custom pin lib/macros */
#include "pin_wrap.h"

/*****************
The error routine that is called if the driver library encounters an error
*****************/
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line) {
}
#endif

/*****************
Clock variables
*****************/
/* System clock rate in Hz */
uint32_t clock_freq;
uint64_t curr_time = 0;

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

#include "gpio_wrap.h"
#include "pwm_wrap.h"
#include "adc_wrap.h"
#include "cmd_wrap.h"

/*****************
Configure I/O
*****************/
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
void ConfigureOuput(void) {
    EnableADC();
    EnablePWM();

    /* Target output */
    MassPeriphInit(tx_output_ports, tx_output_ports_cnt);
    GPIOMassInit(tx_targ_ports, tx_targ_pins, tx_targ_width, PIN_OUT);
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
    UARTEnable(UART0_BASE);*/
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
    for (;;) {
        Loop();
    }
}

/* Custom pin lib/macros */
#include "pin_wrap.h"
#include "mapping_constants.h"
#include "gpio_wrap.h"
#include "adc_wrap.h"
#include "pwm_wrap.h"
#include "cmd_wrap.h"

/* C libraries */
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
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
__error__(char *pcFilename, uint32_t ui32Line) {}
#endif

/*****************
Clock variables
*****************/
/* System clock rate in Hz */
uint32_t clock_freq;
uint64_t curr_time = 0;

/*****************
Configure I/O
*****************/
void ConfigureGPIO(void) {
    /* GPIO ports */
    UARTprintf("Enabling GPIO peripherals.\n");
    MassPeriphInit(picomm_ports, picomm_port_count);
    /* cmd */
    UARTprintf("Initializing command pins.\n");
    GPIOMassInit(cmd_ports, cmd_pins, cmd_width, PIN_IN);
    /* stt */
    UARTprintf("Initializing state pins.\n");
    GPIOMassInit(tistt_ports, tistt_pins, tistt_width, PIN_OUT);
    GPIOMassInit(pistt_ports, pistt_pins, pistt_width, PIN_IN);
}
void ConfigureAnalog(void) {
    UARTprintf("Now configuring ADC\n");
    EnableADC();
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
void SysTick_IntHandler(void) {
    curr_time++;
}
void ConfigureTiming(void) {
    UARTprintf("Settling clock.\n");
    uint32_t clock = SysCtlClockGet();
    UARTprintf("Configuring SysTick interrupt function.\n");
    SysTickIntRegister(SysTick_IntHandler);
    UARTprintf("Set SysTick period to clock speed.\n");
    SysTickPeriodSet(clock_freq);
    UARTprintf("Enable all interrupts then enable SysTick interrupt.\n");
    IntMasterEnable();
    SysTickIntEnable();
    UARTprintf("Enable SysTick.\n");
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
    UARTprintf("Hello. UART configured.\nClock at %ul Hz.\nContinuing with general configuration.\nConfiguring analog I/O.\n", clock_freq);
    ConfigureAnalog();
    UARTprintf("Configuring General Purpose I/O.\n");
    ConfigureGPIO(); //Pi communications
    UARTprintf("Configuring SysTick timing mechanism.\n");
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
        //DigiWritePin(PORT(N), PIN(1), beat?-1:0);
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
    char buff[20];
    for (;;) {
        UARTprintf("abcdef", buff);
    }
}

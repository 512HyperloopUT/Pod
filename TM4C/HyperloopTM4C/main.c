#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "drivers/pinout.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "inc/tm4c1294ncpdt.h"

//COM port with 115200 8N1
#define PTO ROM_GPIOPinTypeGPIOOutput
#define PORT(A) GPIO_PORT ## A ## _BASE
#define PIN(A) GPIO_PIN_ ## A
#define PP(A, B) PORT(A), PIN(B)

// System clock rate in Hz
uint32_t g_ui32SysClock;

// The error routine that is called if the driver library encounters an error
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line) {
}
#endif

// Configure the UART and its pins.  This must be called before UARTprintf()
void ConfigureUART(void) {
	// Enable the GPIO Peripheral used by the UART
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Enable UART0
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	// Configure GPIO Pins for UART mode
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Initialize the UART for console I/O
	UARTStdioConfig(0, 115200, g_ui32SysClock);
}

void ConfigurePiComm() {

	//signal pins
	PTO(PP(C, 6));// PC6
	PTO(PP(H, 2));// PH2

	//MUX selector pins
	PTO(PP(H, 3));// PH3
	PTO(PP(N, 2));// PN2
	PTO(PP(N, 3));// PN3
	PTO(PP(P, 2));// PP2
	PTO(PP(F, 1));// PF1
	PTO(PP(F, 2));// PF2

	//floating point pins

}

int main(void) {
	// Run from the PLL at 120 MHz
	g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120 * 1000000);

	// Configure the device pins
	PinoutSet(false, false);

	// Enable the GPIO pins for the LED D1 (PN1)
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);

	// Initialize the UART
	ConfigureUART();

	int cnt = 0;
	// We are finished.  Hang around flashing D1
	while (1) {
		// Turn on D1.
		LEDWrite(CLP_D1, 1);

		// Delay for a bit
		SysCtlDelay(g_ui32SysClock / 10 / 3);

		UARTprintf("Hello, world: %d!\n", cnt++);

		// Turn off D1
		LEDWrite(CLP_D1, 0);

		// Delay for a bit
		SysCtlDelay(g_ui32SysClock / 10 / 3);
	}
}
#include <stdint.h>
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

/*****************
The error routine that is called if the driver library encounters an error
*****************/
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line) {
}
#endif

/*****************
Pin Data Fetch
*****************/
#define PORT(A) GPIO_PORT ## A ## _BASE
#define PIN(A) GPIO_PIN_ ## A
//define PIN(A) (1 << A)
#define PP(A, B) PORT(A), PIN(B)
/*****************
Pin Init
*****************/
#define PIN_IN 1
#define PIN_OUT 0
#define PTO ROM_GPIOPinTypeGPIOOutput
#define PTI ROM_GPIOPinTypeGPIOInput
#define GPIOPinInit(port, pin, type) type ? PTI(port, pin) : PTO(port, pin)
void GPIOMassInit(uint32_t *ports, uint8_t *pins, uint8_t len, uint8_t input) {
    for (int i = 0; i < len; ++i) {
        GPIOPinInit(ports[i], pins[i], input);
    }
}
/*****************
Digital Pin IO
*****************/
#define PIN_ON 1
#define PIN_OFF 0
#define DigiWritePin GPIOPinWrite
#define DigiReadPin GPIOPinRead
/* Only up to 64 bits, read starts reading to bit 0 */
uint64_t MassRead(uint32_t *ports, uint8_t *pins, uint8_t len) {
    uint64_t val = 0;
    for (int i = 0; i < len; ++i) {
        val |= DigiReadPin(ports[i], pins[i]) << i;
    }
    return val;
}
/* Only up to 64 bits, write starting with bit 0 */
void MassWrite(uint32_t *ports, uint8_t *pins, uint8_t len, uint64_t data) {
    for (int i = 0; i < len; ++i) {
        curr_vals << 1;
        DigiWritePin(ports[i], pins[i], (data >> i) & 1);
    }
}

/*****************
Analog Pin IO
*****************/
#error TODO

/*****************
PIN MAPPINGS (note reverse order)
output:
    stt
        PA7
    odata
        PM6, PM7, PN2, PN3,
        PP2, PP3, PP4, PP5,
        PQ1, PB2 ?, PB3 ?, PD3 ?
        PG1 ?, PN4 ?, PN5 ?, PQ0 ?
input:
    cmd
        PL3, PL4, PL5, PM0,
        PM1, PM2, PM3
    idata
        PC7, PF1, PF2, PF3,
        PG0, PH0, PH1, PH2
        PH3, PK4, PK5, PK6
        PK7, PL0, PL1, PL2
*****************/
uint32_t stt_ports[] = {PORT(A)};
uint8_t stt_pins[] = {PIN(7)};
uint32_t odata_ports[] = {
    {PORT(M)}, {PORT(M)}, {PORT(N)}, {PORT(N)},
    {PORT(P)}, {PORT(P)}, {PORT(P)}, {PORT(P)},
    {PORT(Q)}, {PORT(B)}, {PORT(B)}, {PORT(D)},
    {PORT(G)}, {PORT(N)}, {PORT(N)}, {PORT(Q)}
};
uint8_t odata_pins[] = {
    {PIN(6)}, {PIN(7)}, {PIN(2)}, {PIN(3)},
    {PIN(2)}, {PIN(3)}, {PIN(4)}, {PIN(5)},
    {PIN(1)}, {PIN(2)}, {PIN(3)}, {PIN(3)},
    {PIN(1)}, {PIN(4)}, {PIN(5)}, {PIN(0)}
};
uint32_t cmd_ports[] = {
    {PORT(L)}, {PORT(L)}, {PORT(L)}, {PORT(M)},
    {PORT(M)}, {PORT(M)}, {PORT(M)}
};
uint8_t cmd_pins[] = {
    {PIN(3)}, {PIN(4)}, {PIN(5)}, {PIN(0)},
    {PIN(1)}, {PIN(2)}, {PIN(3)}
};
uint32_t idata_ports[] = {
    {PORT(C)}, {PORT(F)}, {PORT(F)}, {PORT(F)},
    {PORT(G)}, {PORT(H)}, {PORT(H)}, {PORT(H)},
    {PORT(H)}, {PORT(K)}, {PORT(K)}, {PORT(K)},
    {PORT(K)}, {PORT(L)}, {PORT(L)}, {PORT(L)}
};
uint8_t idata_pins[] = {
    {PIN(7)}, {PIN(1)}, {PIN(2)}, {PIN(3)},
    {PIN(0)}, {PIN(0)}, {PIN(1)}, {PIN(2)},
    {PIN(3)}, {PIN(4)}, {PIN(5)}, {PIN(6)},
    {PIN(7)}, {PIN(0)}, {PIN(1)}, {PIN(2)}
};

/*****************
Configure communication with the Raspberry Pi, pin mappings and method.
*****************/
void ConfigurePiComm(void) {
    /* cmd */
    GPIOMassInit(cmd_ports, cmd_pins, 7, PIN_IN);
    /* idata */
    GPIOMassInit(idata_ports, idata_pins, 16, PIN_IN);
    /* stt */
    GPIOPinInit(stt_ports[0], stt_pins[0], PIN_OUT);
    /* odata */
    GPIOMassInit(odata_ports, odata_pins, 16, PIN_OUT);
}

/*****************
Configure the UART and its pins.  This must be called before UARTprintf()
Configures COM port with 115200 8N1
*****************/
void ConfigureUART(void) {
    /* Enable the GPIO Peripheral used by the UART */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    /* Enable UART0 */
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    /* Configure GPIO Pins for UART mode */
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    /* Initialize the UART for console I/O */
    UARTStdioConfig(0, 115200, g_ui32SysClock);
}

/*****************
Attempt to use SysTick for timing data
*****************/
uint64_t curr_time = 0;
void SysTick_IntHandler(void) {
    curr_time++;
}
void ConfigureTiming(void) { //Try for 1ms ticks
    uint32_t clock = SysCtlClockGet();
    SysTickIntRegister(SysTick_IntHandler);
    SysTickPeriodSet(SysCtlClockGet());
    IntMasterEnable();
    SysTickIntEnable();
    SysTickEnable();
}

/*****************
General setup
*****************/
/* System clock rate in Hz */
uint32_t g_ui32SysClock;
void Setup() {
    /* Run from the PLL at 120 MHz */
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120 * 1000000);
    /* Configure the device pins */
    PinoutSet(false, false);
    /* Enable the GPIO pins for the LED D1 (PN1) */
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    /* Initialize the communication pins */
    ConfigureUART(); //UART
    ConfigurePiComm(); //Pi
    #error TODO
    ConfigureTiming(); //Timing
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
    curr_cmd = (uint8_t) MassRead(cmd_ports, cmd_pins, 7);
    /* Read idata */
    last_idata = curr_idata;
    curr_idata = (uint16_t) MassRead(idata_ports, idata_pins, 16);
}
/* Execute instructions */
uint64_t last_exec = 0;
uint16_t exec_sep = 25;
void ExecCmd(void) {
    #error TODO
    if (
            (curr_cmd != last_cmd || curr_idata != last_idata)
            || (/*current_time*/ - last_exec >= exec_sep || exec_beat == 0)
    ) {
        /* Set time data */
        #error TODO
        last_exec = /* current time */;
        /* "Lock" pins */
        WritePin(stt_ports[0], stt_pins[0], PIN_OFF);
        switch (curr_cmd) {
        case 0:
            /* Full reset */
            curr_odata = 0;
            curr_stt = 0;
            break;
        case 1: /* Full instruction */
            if (curr_idata < 0x8000) { /* Simple value fetch */
                #error TODO
            } else { /* Preset instruction */
                CustomFunct(curr_idata);
            }
            break;
        default: /* Writing to motors */
            #error TODO
        }
        /* Read necessary data */
        #error TODO
        curr_odata = /* ????? */;
        /* Write valp */
        MassWrite(odata_ports, odata_pins, 16, curr_odata);
        /* Write s */
        WritePin(stt_ports[0], stt_pins[0], curr_stt);
    }
}

/*****************
Heartbeat
*****************/
uint64_t last_beat = 0;
uint16_t heartbeat_sep = 250;
uint8_t beat = 0;
void Heartbeat(void) {
    #error TODO
    if (curr_time - last_beat >= heartbeat_sep || last_beat == 0) {
        /* Flip state and toggle led */
        beat = !beat;
        #error TODO
        last_beat = /* how does one get the current time */;
        LEDWrite(CLP_D1, on);
    }
}

/*****************
Generic loop function
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

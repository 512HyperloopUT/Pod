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

#define PIDEBUG0

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
#ifndef PIDEBUG0
    #error TODO
#endif

/*****************
Configure communication with the Raspberry Pi, pin mappings and method.
*****************/
/*****************
PIN MAPPINGS (LSb -> MSb)

PWM OUT
PM3
PF1
PF2
PF3
PG0
PM7
PG1
PK4
PK5
PM0

BRAKE OUT
PC6

TIMER CAPTURE
PM1
PM2


PC7
PH2
PH3
PN2
PN3
PP2


ANALOG IN
???? -> PA6
AIN0 -> PE3
AIN1 -> PE2
AIN2 -> PE1
AIN3 -> PE0
AIN4 -> PD7


AIN8 -> PE5
AIN9 -> PE4
AIN13 -> PD2
AIN10 -> PB4
AIN11 -> PB5
AIN16 -> 
AIN17 -> 
AIN18 -> 
AIN19 -> 


output:
    stt
        PA7
    odata
        PM6, PM7, PN2, PN3,
        PP2, PP3, PP4, PP5,
        PQ1, PL3, PL4, PL5
input:
    cmd PM0, PM1, PM2, PM3
    idata
        PC7, PF1, PF2, PF3,
        PG0, PH0, PH1, PH2,
        PH3, PK4, PK5, PK6
*****************/
uint8_t stt_width = 1;
uint32_t stt_ports[] = {PORT(A)};
uint8_t stt_pins[] = {PIN(7)};
uint8_t odata_ports_width = 12;
uint32_t odata_ports[] = {
    {PORT(M)}, {PORT(M)}, {PORT(N)}, {PORT(N)},
    {PORT(P)}, {PORT(P)}, {PORT(P)}, {PORT(P)},
    {PORT(Q)}, {PORT(L)}, {PORT(L)}, {PORT(L)}
};
uint8_t odata_pins[] = {
    {PIN(6)}, {PIN(7)}, {PIN(2)}, {PIN(3)},
    {PIN(2)}, {PIN(3)}, {PIN(4)}, {PIN(5)},
    {PIN(1)}, {PIN(3)}, {PIN(4)}, {PIN(5)}
};
uint8_t cmd_width = 4;
uint32_t cmd_ports[] = {
    {PORT(M)}, {PORT(M)}, {PORT(M)}, {PORT(M)}
};
uint8_t cmd_pins[] = {
    {PIN(0)}, {PIN(1)}, {PIN(2)}, {PIN(3)}
};
uint8_t idata_width = 12;
uint32_t idata_ports[] = {
    {PORT(C)}, {PORT(F)}, {PORT(F)}, {PORT(F)},
    {PORT(G)}, {PORT(H)}, {PORT(H)}, {PORT(H)},
    {PORT(H)}, {PORT(K)}, {PORT(K)}, {PORT(K)}
};
uint8_t idata_pins[] = {
    {PIN(7)}, {PIN(1)}, {PIN(2)}, {PIN(3)},
    {PIN(0)}, {PIN(0)}, {PIN(1)}, {PIN(2)},
    {PIN(3)}, {PIN(4)}, {PIN(5)}, {PIN(6)}
};
void ConfigurePiComm(void) {
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
uint32_t tx_ports[] = {};
uint32_t tx_pins[] = {};
uint32_t rx_ports[] = {};
uint32_t rx_pins[] = {};
void ConfigureOuput(void) {
    /* Analog outputs */
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
    ConfigureOuput(); //Output to home
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
    curr_cmd = (uint8_t) MassRead(cmd_ports, cmd_pins, cmd_width);
    /* Read idata */
    last_idata = curr_idata;
    curr_idata = (uint16_t) MassRead(idata_ports, idata_pins, idata_width);
}
/* Execute instructions */
uint64_t last_exec = 0;
uint16_t exec_sep = 25;
void ExecCmd(void) {
    #ifndef PIDEBUG0
        #error TODO
    if (
            (curr_cmd != last_cmd || curr_idata != last_idata)
            || (curr_time - last_exec >= exec_sep || exec_beat == 0)
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
            #ifndef PIDEBUG0
            if (curr_idata < 0x8000) { /* Simple value fetch */
                #ifndef PIDEBUG0
                    #error TODO
                #endif
            } else { /* Preset instruction */
                CustomFunct(curr_idata);
            }
            #endif
            break;
        default: /* Writing to motors */
            #ifndef PIDEBUG0
                #error TODO
            #endif
        }
        /* Read necessary data */
        #error TODO
        curr_odata = /* ????? */;
        /* Write valp */
        MassWrite(odata_ports, odata_pins, odata_width, curr_odata);
        /* Write s */
        WritePin(stt_ports[0], stt_pins[0], curr_stt);
    }
    
    #endif
    
    MassWrite(stt_ports, stt_pins, stt_width, !cmd);
}

/*****************
Heartbeat
*****************/
uint64_t last_beat = 0;
uint16_t heartbeat_sep = 250;
uint8_t beat = 0;
void Heartbeat(void) {
    if (curr_time - last_beat >= heartbeat_sep || last_beat == 0) {
        /* Flip state and toggle led */
        beat = !beat;
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

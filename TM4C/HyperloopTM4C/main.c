/* C libraries */
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"

#include "utils/uartstdio.h"

#include "drivers/pinout.h"

#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"

/* Run constants */
/* Analog read is needed */
#define ADC_REQ 0
/* Analog write is needed */
#define PWM_REQ
/* Only stt and cmd IO ports */
#define PI_SIMPLE 1
/* All 4 ports needed */
#define PI_ADVANCED 0

/*****************
The error routine that is called if the driver library encounters an error
*****************/
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line) {
}
#endif

/*****************
Configure SysTick for use with timing data
*****************/
uint64_t curr_time = 0;
void SysTick_IntHandler(void) { curr_time++; }
void ConfigureTiming(void) {
    /* Settle clock */
    uint32_t clock = SysCtlClockGet();
    /* Make a function available for handling the SysTick */
    SysTickIntRegister(SysTick_IntHandler);
    /* Equivalent Clock speed and SysTick */
    SysTickPeriodSet(SysCtlClockGet());
    /* Enable interrupts */
    IntMasterEnable();
    /* Enables SysTick interrupts */
    SysTickIntEnable();
    /* Enable the SysTick itself */
    SysTickEnable();
}

/*****************
Pin Mapping Macros
*****************/
#define SYSPORT(A) SYSCTL_PERIPH_GPIO ## A
#define PORT(A) GPIO_PORT ## A ## _BASE
#define PIN(A) GPIO_PIN_ ## A
#define PP(A, B) PORT(A), PIN(B)
/*****************
Pin GPIO Init Macros and Functions
*****************/
#define PIN_IN 1
#define PIN_OUT 0
#define PTO GPIOPinTypeGPIOOutput
#define PTI GPIOPinTypeGPIOInput
#define GPIOPinInit(port, pin, type) type ? PTI(port, pin) : PTO(port, pin)
/* Enable the ports for GPIO */
void GPIOPeriphInit(uint32_t* ports, uint8_t len) {
    int i;
    for (i = 0; i < len; ++i) {
        SysCtlPeriphEnable(ports[i]);
        while (!SysCtlPeriphReady(ports[i]));
    }
}
void GPIOMassInit(uint32_t *ports, uint8_t *pins, uint8_t len, uint8_t input) {
    int i;
    for (i = 0; i < len; ++i) {
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
#if ADC_REQ == 1 || PWM_REQ == 1
    #error "Analog writes on the TODO list"
#endif

/*****************
Configure communication with the Raspberry Pi, pin mappings and method.
*****************/
/*****************
PIN MAPPINGS (LSb -> MSb)
stt
    PA7
odata
    PL1, PL2, PL3, PL4,
    PL5, PM0, PM1, PM2,
    PM3, PM6, PM7, PN2
cmd
    PK5, PK6, PK7, PL0
idata
    PC6, PC7, PN3, PF2,
    PF3, PG0, PG1, PH0,
    PH1, PH2, PH3, PK4
*****************/
uint8_t stt_width = 1;
uint32_t stt_ports[] = {PORT(A)};
uint8_t stt_pins[] = {PIN(7)};
uint8_t odata_ports_width = 12;
uint32_t odata_ports[] = {
    PORT(L), PORT(L), PORT(L), PORT(L),
    PORT(L), PORT(M), PORT(M), PORT(M),
    PORT(M), PORT(M), PORT(M), PORT(N)
};
uint8_t odata_pins[] = {
    PIN(1), PIN(2), PIN(3), PIN(4),
    PIN(5), PIN(0), PIN(1), PIN(2),
    PIN(3), PIN(6), PIN(7), PIN(2)
};
uint8_t cmd_width = 4;
uint32_t cmd_ports[] = {
    PORT(K), PORT(K), PORT(K), PORT(L)
};
uint8_t cmd_pins[] = {
    PIN(5), PIN(6), PIN(7), PIN(0)
};
uint8_t idata_width = 12;
uint32_t idata_ports[] = {
    PORT(C), PORT(C), PORT(N), PORT(F),
    PORT(F), PORT(G), PORT(G), PORT(H),
    PORT(H), PORT(H), PORT(H), PORT(H)
};
uint8_t idata_pins[] = {
    PIN(6), PIN(7), PIN(3), PIN(2),
    PIN(3), PIN(0), PIN(1), PIN(0),
    PIN(1), PIN(2), PIN(3), PIN(4)
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
/*****************
Mappings to other things
tx
    PF1
tx_targ
    PP2, PP3, PP5, PQ1
rx
    PA6 -> ????, PB4 -> AIN10,
    PB5 -> AIN11, PD2 -> AIN13,
    PD7 -> AIN4, PE0 -> AIN3,
    PE1 -> AIN2, PE2 -> AIN1,
    PE3 -> AIN0, PE4 -> AIN9,
    PE5 -> AIN8, PK0 -> AIN16,
    PK1 -> AIN17, PK2 -> AIN18,
    PK3 -> AIN19
*****************/
uint8_t tx_width = 1;
uint32_t tx_ports[] = {
    
};
uint8_t tx_pins[] = {
    
};
uint8_t tx_targ_width = 4;
uint32_t tx_targ_ports[] = {
    
};
uint8_t tx__targ_pins[] = {
    
};
uint8_t rx_width = 0;
uint32_t rx_ports[] = {
    
};
uint8_t rx_pins[] = {
    
};
void ConfigureOuput(void) {
    /* Analog outputs */
    #if ANALOG_REQ == 1
        #error "Analog output must be configured."
    #endif
    #if ADC_REQ == 1
        #error "Analog read must be configured."
    #endif
}

/*****************
Configure GPIO peripherals related to the above issues
*****************/
uint8_t pi_port_count = 11;
uint32_t pi_comm_ports[] = {
    SYSPORT(A), SYSPORT(C), SYSPORT(F), SYSPORT(G),
    SYSPORT(H), SYSPORT(K), SYSPORT(L), SYSPORT(M),
    SYSPORT(N)
};
void ConfigurePeripherals(void) {
    /* Setup peripherals */
    GPIOPeriphInit(pi_comm_ports, pi_port_count);
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
#if PI_ADVANCED == 1
    if (
            (curr_cmd != last_cmd || curr_idata != last_idata)
            || (curr_time - last_exec >= exec_sep || exec_beat == 0)
    ) {
        /* Set time data */
        last_exec = curr_time;
        /* "Lock" pins */
        WritePin(stt_ports[0], stt_pins[0], PIN_OFF);
        switch (curr_cmd) {
        case 0:
        case 1: /* Read instruction */
            if (curr_idata < 0x800) { /* Simple value fetch */
                #if ADC_REQ == 1
                    #error "Must initialize and configure ADC"
                #endif
            } else { /* Preset instruction */
                CustomFunct(curr_idata);
            }
            curr_stt = curr_cmd;
            break;
        default: /* Writing to motors */
            #if PWM_REQ == 1
                #error "ADC Not Setup"
            #endif
        }
        /* Read necessary data */
        #if ADC_REQ == 1
            #error "ADC not set up"
        #endif
        curr_odata = ReadAnalogPin(/* Decoded pin here */);
        /* Write valp */
        MassWrite(odata_ports, odata_pins, odata_width, curr_odata);
        /* Write s */
        WritePin(stt_ports[0], stt_pins[0], curr_stt);
    }
#elif PI_SIMPLE == 1
    MassWrite(stt_ports, stt_pins, stt_width, cmd ? 0 : 1);
#else
    #error "No Pi communication protocol established"
#endif
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

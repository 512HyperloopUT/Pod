# Documentation for the TM4C code

## Files

 - main
 - pin_wrap
 - mapping_constants
 - cmd_wrap
 - gpio_wrap
 - adc_wrap
 - pwm_wrap
	
## Main

## Pin wrapping

Name: pin_wrap.h

A series of macros for ease of access for GPIO ports and pins

## Mapping constants

Name: mapping_constants.h, mapping_constants.c

Variables storing the information needed to write data with

Three ports are needed:

tistt for the state of the TM4C

pistt for the state of the Pi

cmd for the currently loaded command from the Pi

### tistt_width

Type: uint8_t

Number of pins for the TM4C state port (currently 1)

### tistt_ports

Type: uint32_t[]

Port number for the ith pin in the TM4C state port

### tistt_pins

Type: uint8_t[]

Pin number for the ith pin in the TM4C state port

### pistt_width

Type: uint8_t

Number of pins for the Pi state port (currently 1)

### pistt_ports

Type: uint32_t[]

Port number for the ith pin in the Pi state port

### pistt_pins

Type: uint8_t[]

Pin number for the ith pin in the Pi state port

### cmd_width

Type: uint8_t

Number of pins for the command port (currently 5)

### cmd_ports

Type: uint32_t[]

Port number for the ith pin in the command port

### cmd_pins

Type: uint8_t[]

Pin number for the ith pin in the command port

### picomm_port_count

Type: uint8_t

Number of ports needed for GPIO.

### picomm_ports

Type: uint32_t[]

Array of ports used for the communication between the pi and tm4c.

## Command logic

Files: cmd_wrap.h, cmd_wrap.c

### ReadCmd

Return type: None

Arguments: None

A call to prepare for ExecCmd. A meaningful response from ExecCmd requires this function to be called.

Determines if the command is valid based on input and then reads the command if it is valid.

### ExecCmd

Return type: None

Arguments: None

Executes command based on data stored by ReadCmd. Only meaningful after calling ReadCmd.

Reads a sensor's value if it is a new command.

## GPIO functions

### MassPeriphInit

Returns: None

Arguments:
 - const uint32_t *ports
    - Array of ports
 - uint8_t len
    - Length of the array of ports

Enable a series of peripheral ports for GPIO.

### GPIOMassInit

Returns: None

Arguments:
 - const uint32_t *ports
    - array of port pin's port
 - const uint8_t *pins
    - array of port pin's pin
 - uint8_t len
    - number of pin in the port to intialize
 - uint8_t direction
    - pin in or pin out. A series of macros, PIN_IN and PIN_OUT for this.

Initialize a "port".

### MassRead

Returns: uint64_t

Arguments:
 - const uint32_t *ports
    - array of port pin's port
 - const uint8_t *pins
    - array of port pin's pin
 - uint8_t len
    - number of pin in the port to intialize

Read a "port".

### MassWrite

Returns: None

Arguments:
 - const uint32_t *ports
    - array of port pin's port
 - const uint8_t *pins
    - array of port pin's pin
 - uint8_t len
    - number of pin in the port to intialize
 - uint64_t data
    - Value to write to the port

Write to a "port".

## PWM writing functions

### EnablePWM

Return type: None

Arguments: None

Enable a timer based PWM pin.

### WritePWM

Return type: None

Arguments:
 - uint32_t ui32Port
    - Port to write to
 - uint8_t ui8Pins
    - Pin to write to
 - uint16_t value
    - Value to write. Part of 1000
 
Sets a pwm with cycle of 100 timer cycles and value/1000.

## ADC writing functions

### EnableADC

Returns: None

Arguments: None

Enable the ADC peripherals

### MapADCRead

Returns: int

Arguments:
 - uint32_t val
    - Id of sensor to read

Read a port given an id.
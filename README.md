# mcp2551_driver

Module to initialize and drive the mcp2551 CANbus controller.

## Notes

* Currently only standard size messages can be sent and received. It is currently unknow what will happen if an extended message is sent to the bus. This will be fixed once extended messages are set up correctly in the code.
* Only RX0 receive and TX0 buffer have been set up to work. The other RX and TX buffers will be set up to work in the near future.

## Installation

The device needs to be initialised over SPI before it can be used. The initialising will restart the IC and place in into normal mode.

Init will also set up any interrupts.

NOTE There currently isn't a way to change the settings once the device is in normal mode. This is because the library can't change the mode (will be added at a later point).1

## Usage

The SPI pins and modules must be set up and the initialised before the mcp2515 module is initalised. The mcp init function will call a number of SPI exchange functions.

The cs_low and cs_high will need need filling in with appropriate pin changing actions.

The driver require the following spi functions to work:
* void spi_driver_cs_low(void)
* void spi_driver_cs_high(void)
* uint8_t spi_driver_exchange(uint8_t data)

## Testing

All functionality is tested using Ceedling.

```
ceedling
```

## History

### 9th September 2019

### 6th September 2019

* Transmission of messages is now working well.
  * There are still a number of boundary checks and features that need to be implemented, but these will be implemented as needed.
  * Only a single buffer (buffer 1) is currently implemented.
* Receiving messages is not yet implemented. This is the next step in the development of the module.

### 16th September 2019

* Receiving of messages is now set up and is working correctly.
  * Only buffer is set up to be working.
  * Only standard messages are working. It is unknown what will happen if an ext message is sent to a bus that one of these nodes are connected to.
 
### 8th October 2019

* Interrupts are now set up and working.
    * Only rx0 buffer full is set up to work
    * Only INT pin is set up as an interrupt output. The others will be set up, but in the near future.


# mcp2551_driver

Module to initialize and drive the mcp2551 CANbus controller.

## Notes

* Currently only standard size messages can be sent and received. It is currently unknow what will happen if an extended message is sent to the bus. This will be fixed once extended messages are set up correctly in the code.

## Installation

The device needs to be initialised over SPI before it can be used. The initialising will restart the IC and place in into normal mode.

Init will also set up any interrupts.

NOTE There currently isn't a way to change the settings once the device is in normal mode. This is because the library can't change the mode (will be added at a later point).

## Usage

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

### 4th September 2019

* Interrupt pin is set up and working.
    * Currently only working to interrupt when rx0 buffer is full.
    * This helps remove the need for polling over SPI to see if the buffer is full.
* Interrupt pin is set up during init, so it will be working from then on.

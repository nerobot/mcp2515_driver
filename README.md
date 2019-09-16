# mcp2551_driver

Module to initialize and drive the mcp2551 CANbus controller.

## Notes

* Currently only standard size messages can be sent and received. It is currently unknow what will happen if an extended message is sent to the bus. This will be fixed once extended messages are set up correctly in the code.

## Installation

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

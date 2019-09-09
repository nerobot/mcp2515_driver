# mcp2551_driver

## History

### 9th September 2019

### 6th September 2019

* Transmission of messages is now working well.
  * There are still a number of boundary checks and features that need to be implemented, but these will be implemented as needed.
  * Only a single buffer (buffer 1) is currently implemented.
* Receiving messages is not yet implemented. This is the next step in the development of the module.

## TODO

* Implement use of both tx buffers
  * Currently only tx buffer 1 is being used
* Implement the ability to use interrupts used in setting up and sending messages.
* Transmission is currently blocking the rest of the code if there is an error sending a message.
  * This needs fixing.

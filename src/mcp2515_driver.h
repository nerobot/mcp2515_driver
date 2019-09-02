#ifndef _MCP2515_DRIVER_H
#define _MCP2515_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "spi_driver.h"
#include "mcp2515_reg.h"

void mcp2515_init(void);

/**
 * @brief           Causes a reset via SPI command.
 *
 * @return true     Reset was successful and device is in config mode
 * @return false    Reset was not successful
 *
 */
// TODO: Check if there are any other potential failure modes to check
bool mcp2515_driver_reset(void);

/**
 * @brief               Sets up the baudrate given a rate and clock speed
 *
 * @param can_speed     Macro defined baudrate
 * @param can_clock     Clock speed of the module
 * @return true
 * @return false        Incorrect speed or rate requested
 *
 * @warning             Not all speeds are implemented
 */
bool mcp2515_driver_set_baudrate(uint8_t can_speed, uint8_t can_clock);

/**
 * @brief               Will initialise all of the can buffers to zero.
 *
 * @warning             Only tx buffers are initialised in the current
 * implementation
 *
 */
void mcp2515_driver_init_can_buffers(void);

bool mcp2515_driver_send_msg_buffer(uint8_t can_id, uint8_t ext,
                                    uint8_t buf_size, uint8_t * tx_buf);

#endif // _MCP2551_DRIVER_H

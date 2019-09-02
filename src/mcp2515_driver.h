#ifndef _MCP2515_DRIVER_H
#define _MCP2515_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "spi_driver.h"
#include "mcp2515_reg.h"

/**
 * @brief           Causes a reset via SPI command.
 *
 * @return true     Reset was successful and device is in config mode
 * @return false    Reset was not successful
 *
 */
// TODO: Check if there are any other potential failure modes to check
bool mcp2515_driver_reset(void);

#endif // _MCP2551_DRIVER_H

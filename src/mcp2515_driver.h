/**
 * Funtinality to add:
 * * Sending and recieving ext messages
 * * Sending and recieving variable message sizes
 * * Set up and use interrupt and txNrts lines
 * * Use both rx buffers
 * * Use both tx buffers
 * * Look into potential low power modes
 * * Implement filters
 * * Implement masks
 * * Implement the ability to use one shot mode
 */

#ifndef _MCP2515_DRIVER_H
#define _MCP2515_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "spi_driver.h"
#include "mcp2515_reg.h"

bool mcp2515_init(void);

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

/**
 * @brief               Sends the can message
 *
 * @param can_id
 * @param ext
 * @param buf_size
 * @param tx_buf
 * @return true
 * @return false
 *
 * @warning             No check done on size of buf
 * @note                Currently only uses tx_buf 1
 * @note                Spi needs to be set up before this is called
 */
bool mcp2515_driver_send_msg_buffer(uint16_t can_id, uint8_t ext,
                                    uint8_t buf_size, uint8_t * tx_buf);

bool mcp2515_rx0_is_full(void);

void mcp2515_driver_read_can_message(uint8_t * id, uint8_t * len,
                                     uint8_t * read_buf);

void mcp2515_driver_clear_rx0if(void);

bool mcp2515_set_rx0ie(void);

#endif // _MCP2551_DRIVER_H

#include "mcp2515_driver.h"

void mcp2515_driver_reset(void)
{
    spi_driver_exchange(MCP_RESET);
}
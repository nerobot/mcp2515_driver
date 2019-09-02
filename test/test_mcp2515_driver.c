#include "unity.h"
#include "mcp2515_driver.h"
#include "mock_spi_driver.h"
#include <stdint.h>
#include "mcp2515_reg.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_reset_will_send_reset_command(void)
{
    spi_driver_exchange_ExpectAndReturn(MCP_RESET, 0);
    mcp2515_driver_reset();
}
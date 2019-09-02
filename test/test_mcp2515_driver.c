#include "unity.h"
#include "mcp2515_driver.h"
#include "mock_spi_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include "mcp2515_reg.h"

uint8_t reset_param[4] = {0, 0, 0, 0b10000000};

static bool reset(uint8_t param[])
{
    spi_driver_exchange_ExpectAndReturn(MCP_RESET, param[0]);
    spi_driver_exchange_ExpectAndReturn(MCP_READ, param[1]);
    spi_driver_exchange_ExpectAndReturn(MCP_CANSTAT, param[2]);
    spi_driver_exchange_ExpectAndReturn(0, param[3]);

    return mcp2515_driver_reset();
}

void setUp(void)
{
}

void tearDown(void)
{
}

void test_reset_all_ok(void)
{
    reset(reset_param);
}

void test_reset_all_ok_return_true(void)
{
    bool success = reset(reset_param);
    TEST_ASSERT(success);
}

void test_after_reset_device_not_in_config_mode_return_false(void)
{
    reset_param[3] = 0;
    bool success   = reset(reset_param);
    TEST_ASSERT_FALSE(success);
}

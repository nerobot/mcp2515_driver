#include "mcp2515_driver.h"

// TODO: Check if the following two functions would be better being called from
// spi_driver
static inline void cs_low(void)
{
    // TODO: implement
}

static inline void cs_high(void)
{
    // TODO: implement
}

bool mcp2515_driver_reset(void)
{
    uint8_t canstat = 0;
    cs_low();
    spi_driver_exchange(MCP_RESET);
    cs_high();

    cs_low();
    spi_driver_exchange(MCP_READ);
    spi_driver_exchange(MCP_CANSTAT);
    canstat = spi_driver_exchange(0);
    cs_high();

    if (0b10000000 == (canstat & 0b11100000))
    {
        return true;
    }
    return false;
}
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

static bool is_can_clock_within_bounds(uint8_t value)
{
    if (value > MCP_8MHZ)
    {
        return false;
    }
    return true;
}

static bool is_can_speed_within_bounds(uint8_t value)
{
    if (value > CAN_1000KBPS)
    {
        return false;
    }
    return true;
}

static void set_register(uint8_t address, uint8_t value)
{
    spi_driver_exchange(MCP_WRITE);
    spi_driver_exchange(address);
    spi_driver_exchange(value);
}

void mcp2515_init(void)
{
    cs_high();
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

// TODO: Implement all combinations of speeds
bool mcp2515_driver_set_baudrate(uint8_t can_speed, uint8_t can_clock)
{
    if (false == is_can_clock_within_bounds(can_clock))
    {
        return false;
    }
    if (false == is_can_speed_within_bounds(can_speed))
    {
        return false;
    }

    uint8_t cfg1, cfg2, cfg3;
    bool set = true;

    switch (can_clock)
    {
    case MCP_16MHZ:
        switch (can_speed)
        {
        case CAN_5KBPS:
            cfg1 = MCP_16MHz_5kBPS_CFG1;
            cfg2 = MCP_16MHz_5kBPS_CFG2;
            cfg3 = MCP_16MHz_5kBPS_CFG3;
            break;

        default:
            set = false;
            break;
        }
        break;

    default:
        set = false;
        break;
    }

    if (true == set)
    {
        set_register(MCP_CNF1, cfg1);
        set_register(MCP_CNF2, cfg2);
        set_register(MCP_CNF3, cfg3);
    }

    return true;
}

// TODO: Implement rx buffers
void mcp2515_driver_init_can_buffers(void)
{
    uint8_t address0 = MCP_TXB0CTRL;
    uint8_t address1 = MCP_TXB1CTRL;
    uint8_t address2 = MCP_TXB2CTRL;

    uint8_t i = 0;

    for (i = 0; i < 14; i++)
    {
        set_register(address0++, 0);
        set_register(address1++, 0);
        set_register(address2++, 0);
    }
}

bool mcp2515_driver_send_msg_buffer(uint8_t can_id, uint8_t ext,
                                    uint8_t buf_size, uint8_t * tx_buf)
{
}
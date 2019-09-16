#include "mcp2515_driver.h"

static inline void cs_low(void)
{
    spi_driver_cs_low();
}

static inline void cs_high(void)
{
    spi_driver_cs_high();
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
    cs_low();
    spi_driver_exchange(MCP_WRITE);
    spi_driver_exchange(address);
    spi_driver_exchange(value);
    cs_high();
}

static uint8_t read_register(uint8_t address)
{
    cs_low();
    spi_driver_exchange(MCP_READ);
    spi_driver_exchange(address);
    uint8_t value = spi_driver_exchange(0);
    cs_high();
    return value;
}

static inline bool check_bit(uint8_t value, uint8_t bit)
{
    return (value & (1 << bit));
}

static inline uint8_t clear_bit(uint8_t value, uint8_t bit)
{
    return (value & ~(1 << bit));
}

// TODO Add ability to change / add filters.
// This might be better to keep as it is for here and add a seperate function for adding filters
static void init_rx_buffers(void)
{
    // all filters off, rollover disabled
    set_register(MCP_RXB0CTRL, 0b01100000);
    // all filters off
    set_register(MCP_RXB1CTRL, 0b01100000);
}

static uint8_t read_status(void)
{
    cs_low();
    spi_driver_exchange(MCP_READ_STATUS);
    uint8_t status = spi_driver_exchange(0);
    cs_high();
    return status;
}

static bool change_mode(uint8_t mode)
{
    uint8_t canctrl = read_register(MCP_CANCTRL);
    canctrl &= ((mode << 5) & 0xFF);
    set_register(MCP_CANCTRL, mode);    
    // TODO check that canctrl has been changed as required
    // Return if not the same
}

static uint8_t read_mode(void)
{
	uint8_t mode = read_register(MCP_CANSTAT);
	mode >>= 5;
	return mode;	
}

// TODO: Do I want to add SPI init here?
bool mcp2515_init(void)
{
    cs_high();
    if (false == mcp2515_driver_reset())
    {
        return false;
    }
    mcp2515_driver_set_baudrate(CAN_5KBPS, MCP_16MHZ);

    // Set up rx buffers
    init_rx_buffers();

    // Set into normal mode
    // TODO: Add funtionality to check if the device went into the correct mode
   change_mode(0);
   if (0 != read_mode())
   {
	return false;
   }
   return true;
}

bool mcp2515_driver_reset(void)
{
    uint8_t canstat = 0;
    cs_low();
    spi_driver_exchange(MCP_RESET);
    cs_high();

    // Checking to see if the device is now in config mode after the reset
    // TODO: The calls below should be inside a function call to remove the
    // repeated code
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
// TODO: Double check the calculations for the speed macros
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

        case CAN_1000KBPS:
            cfg1 = MCP_16MHz_1000kBPS_CFG1;
            cfg2 = MCP_16MHz_1000kBPS_CFG2;
            cfg3 = MCP_16MHz_1000kBPS_CFG3;
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

// TODO: Implement rx buffers??
// Not actually sure if the above is needed or not
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

// TODO: Add ability to send ext message
bool mcp2515_driver_send_msg_buffer(uint16_t can_id, uint8_t ext,
                                    uint8_t buf_size, uint8_t * tx_buf)
{
    uint8_t txb0ctrl          = 0;
    bool tx_success           = false;
    bool tx_error             = false;
    bool tx_arbitration_error = false;
    bool no_errors            = false;

    if (0 == buf_size)
    {
        return false;
    }

    if (buf_size > MCP_MAX_STD_BUF_SIZE)
    {
        return false;
    }

    do
    {
        txb0ctrl = read_register(MCP_TXB0CTRL);
    } while (true == check_bit(txb0ctrl, 3));

    // Set up tx control registers
    // TODO: Check if the following can be improved
    set_register(MCP_TXB0SIDH, (uint8_t)(can_id >> 3));
    set_register(MCP_TXB0SIDL, (0b11100000 & (uint8_t)(can_id << 5)));
    set_register(MCP_TXB0EID8, 0);
    set_register(MCP_TXB0EID0, 0);
    set_register(MCP_TXB0DLC, buf_size);

    // Fill up buffers
    // TODO: Emprove efficiency
    set_register(MCP_TXB0D0, tx_buf[0]);
    set_register(MCP_TXB0D1, tx_buf[1]);
    set_register(MCP_TXB0D2, tx_buf[2]);
    set_register(MCP_TXB0D3, tx_buf[3]);
    set_register(MCP_TXB0D4, tx_buf[4]);
    set_register(MCP_TXB0D5, tx_buf[5]);
    set_register(MCP_TXB0D6, tx_buf[6]);
    set_register(MCP_TXB0D7, tx_buf[7]);

    // Send tx
    set_register(MCP_TXB0CTRL, 0b00001000);

    // Checking if tx was successful
    do
    {
        txb0ctrl             = read_register(MCP_TXB0CTRL);
        tx_success           = !check_bit(txb0ctrl, 3);
        tx_error             = check_bit(txb0ctrl, 4);
        tx_arbitration_error = check_bit(txb0ctrl, 5);

        // no_errors = tx_error | tx_arbitration_error;
    } while (!tx_success & !tx_error & !tx_arbitration_error);
    // TODO: Improve the while logic above at isnt great to read

    // TODO: Implement a more useful return value
    if ((true == tx_arbitration_error))
    {
        return false;
    }
    if ((true == tx_error))
    {
        return false;
    }

    return true;
}

// TODO Remane to something better
bool mcp2515_rx0_is_full(void)
{
    uint8_t status = read_status();

    bool rx0_full = check_bit(status, 0);
    return rx0_full;
}

// TODO Remove all the direct spi calls so that they are kept in the same place
void mcp2515_driver_read_can_message(uint8_t * id, uint8_t * len,
                                     uint8_t * read_buf)
{
    uint8_t buf[14];
    uint8_t i = 0;

    cs_low();
    spi_driver_exchange(MCP_READ_BUF_RX0);

    // Message ID
    // TODO Implement ext message ID as well
    buf[0] = spi_driver_exchange(0);
    buf[1] = spi_driver_exchange(0);
    *id    = (uint8_t)((buf[0] << 3) + (buf[1] >> 5));

    // Extended ID - currently not implemented
    buf[2] = spi_driver_exchange(0);
    buf[3] = spi_driver_exchange(0);

    // Message length
    buf[4] = spi_driver_exchange(0);
    *len   = buf[4];

    // Reading the data
    // TODO Only read as many bytes as is needed
    uint8_t * p_read_buf = read_buf;
    for (i = 0; i < 8; i++)
    {
        *p_read_buf++ = spi_driver_exchange(0);
    }

    cs_high();
}

// TODO Implement a read_status function
void mcp2515_driver_clear_rx0if(void)
{
    uint8_t status = read_status();

    status = clear_bit(status, 0);
    set_register(MCP_CANINTF, status);
}

#include "unity.h"
#include "mcp2515_driver.h"
#include "mock_spi_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include "mcp2515_reg.h"

uint8_t reset_param[4] = {0, 0, 0, 0b10000000};
uint8_t can_clock      = MCP_16MHZ;
uint8_t can_speed      = CAN_1000KBPS;
uint16_t can_id        = 0x0070;
uint8_t buf_size       = 8;
uint8_t tx_buf[8]      = {1, 2, 3, 4, 5, 6, 7, 8};

static bool reset(uint8_t param[])
{
    spi_driver_exchange_ExpectAndReturn(MCP_RESET, param[0]);
    spi_driver_exchange_ExpectAndReturn(MCP_READ, param[1]);
    spi_driver_exchange_ExpectAndReturn(MCP_CANSTAT, param[2]);
    spi_driver_exchange_ExpectAndReturn(0, param[3]);

    // return mcp2515_driver_reset();
}

static void set_register(uint8_t address, uint8_t value)
{
    spi_driver_exchange_ExpectAndReturn(MCP_WRITE, 0);
    spi_driver_exchange_ExpectAndReturn(address, 0);
    spi_driver_exchange_ExpectAndReturn(value, 0);
}

static void read_register(uint8_t address, uint8_t expected)
{
    spi_driver_exchange_ExpectAndReturn(MCP_READ, 0);
    spi_driver_exchange_ExpectAndReturn(address, 0);
    spi_driver_exchange_ExpectAndReturn(0, expected);
}

static void send_buffer(void)
{
    // Fill up buffers
    set_register(MCP_TXB0D0, tx_buf[0]);
    set_register(MCP_TXB0D1, tx_buf[1]);
    set_register(MCP_TXB0D2, tx_buf[2]);
    set_register(MCP_TXB0D3, tx_buf[3]);
    set_register(MCP_TXB0D4, tx_buf[4]);
    set_register(MCP_TXB0D5, tx_buf[5]);
    set_register(MCP_TXB0D6, tx_buf[6]);
    set_register(MCP_TXB0D7, tx_buf[7]);
}

void setUp(void)
{
    reset(reset_param);

    set_register(MCP_CNF1, MCP_16MHz_1000kBPS_CFG1);
    set_register(MCP_CNF2, MCP_16MHz_1000kBPS_CFG2);
    set_register(MCP_CNF3, MCP_16MHz_1000kBPS_CFG3);

    set_register(0x0F, 0x00);

    mcp2515_init();
}

void tearDown(void)
{
}

void test_init(void)
{
}

void test_reset_all_ok(void)
{
    reset(reset_param);
    mcp2515_driver_reset();
}

void test_reset_all_ok_return_true(void)
{
    reset(reset_param);
    bool success = mcp2515_driver_reset();
    TEST_ASSERT(success);
}

void test_after_reset_device_not_in_config_mode_return_false(void)
{
    reset_param[3] = 0;
    reset(reset_param);
    bool success = mcp2515_driver_reset();
    TEST_ASSERT_FALSE(success);
}

void test_set_baudrate_can_clock_out_off_bounds_return_false(void)
{
    bool success = true;
    success      = mcp2515_driver_set_baudrate(can_speed, 3);
    TEST_ASSERT_FALSE(success);
}

void test_set_baudrate_can_speed_out_off_bounds_return_false(void)
{
    bool success = true;
    success      = mcp2515_driver_set_baudrate(19, can_clock);
    TEST_ASSERT_FALSE(success);
}

void test_set_baudrate_5kbps_16mhz(void)
{
    bool success = false;
    can_speed    = CAN_5KBPS;
    can_clock    = MCP_16MHZ;

    set_register(MCP_CNF1, MCP_16MHz_5kBPS_CFG1);
    set_register(MCP_CNF2, MCP_16MHz_5kBPS_CFG2);
    set_register(MCP_CNF3, MCP_16MHz_5kBPS_CFG3);

    success = mcp2515_driver_set_baudrate(can_speed, can_clock);
    TEST_ASSERT(success);
}

void test_init_can_buffers_will_zero_to_all_txctrl_regs(void)
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

    mcp2515_driver_init_can_buffers();
}

void test_tx1_si_all_ok(void)
{
    read_register(MCP_TXB0CTRL, 0x00);

    // Set up tx control registers
    set_register(MCP_TXB0SIDH, (uint8_t)(can_id >> 3));
    set_register(MCP_TXB0SIDL, (0b11100000 & (uint8_t)(can_id << 5)));
    set_register(MCP_TXB0EID8, 0);
    set_register(MCP_TXB0EID0, 0);
    set_register(MCP_TXB0DLC, buf_size);

    send_buffer();

    // Send tx
    set_register(MCP_TXB0CTRL, 0b00001000);

    // Checking if the
    read_register(MCP_TXB0CTRL, 0x00);

    bool success = mcp2515_driver_send_msg_buffer(can_id, 0, buf_size, tx_buf);
    TEST_ASSERT(success);
}

void test_tx1_si_wait_until_buffer_empty_before_trying_to_setup_tx(void)
{
    uint8_t i = 0;
    for (i = 0; i < 88; i++)
    {
        read_register(MCP_TXB0CTRL, 0b00001000);
    }
    read_register(MCP_TXB0CTRL, 0x00);

    // Set up tx control registers
    set_register(MCP_TXB0SIDH, (uint8_t)(can_id >> 3));
    set_register(MCP_TXB0SIDL, (0b11100000 & (uint8_t)(can_id << 5)));
    set_register(MCP_TXB0EID8, 0);
    set_register(MCP_TXB0EID0, 0);
    set_register(MCP_TXB0DLC, buf_size);

    send_buffer();

    // Send tx
    set_register(MCP_TXB0CTRL, 0b00001000);

    // Checking if the
    read_register(MCP_TXB0CTRL, 0x00);

    bool success = mcp2515_driver_send_msg_buffer(can_id, 0, buf_size, tx_buf);
    TEST_ASSERT(success);
}

void test_tx1_si_delay_in_tx_but_no_error(void)
{
    read_register(MCP_TXB0CTRL, 0x00);

    // Set up tx control registers
    set_register(MCP_TXB0SIDH, (uint8_t)(can_id >> 3));
    set_register(MCP_TXB0SIDL, (0b11100000 & (uint8_t)(can_id << 5)));
    set_register(MCP_TXB0EID8, 0);
    set_register(MCP_TXB0EID0, 0);
    set_register(MCP_TXB0DLC, buf_size);

    send_buffer();

    // Send tx
    set_register(MCP_TXB0CTRL, 0b00001000);

    // Checking if tx has transmitted and if there are any errors
    read_register(MCP_TXB0CTRL, 0b00001000);
    read_register(MCP_TXB0CTRL, 0b00000000);

    bool success = mcp2515_driver_send_msg_buffer(can_id, 0, buf_size, tx_buf);
    TEST_ASSERT(success);
}

void test_tx1_si_delay_in_tx_and_arb_error(void)
{
    read_register(MCP_TXB0CTRL, 0x00);

    // Set up tx control registers
    set_register(MCP_TXB0SIDH, (uint8_t)(can_id >> 3));
    set_register(MCP_TXB0SIDL, (0b11100000 & (uint8_t)(can_id << 5)));
    set_register(MCP_TXB0EID8, 0);
    set_register(MCP_TXB0EID0, 0);
    set_register(MCP_TXB0DLC, buf_size);

    send_buffer();

    // Send tx
    set_register(MCP_TXB0CTRL, 0b00001000);

    // Checking if tx has transmitted and if there are any errors
    read_register(MCP_TXB0CTRL, 0b00101000);

    bool success = mcp2515_driver_send_msg_buffer(can_id, 0, buf_size, tx_buf);
    TEST_ASSERT_FALSE(success);
}

void test_tx1_si_delay_in_tx_and_message_error(void)
{
    read_register(MCP_TXB0CTRL, 0x00);

    // Set up tx control registers
    set_register(MCP_TXB0SIDH, (uint8_t)(can_id >> 3));
    set_register(MCP_TXB0SIDL, (0b11100000 & (uint8_t)(can_id << 5)));
    set_register(MCP_TXB0EID8, 0);
    set_register(MCP_TXB0EID0, 0);
    set_register(MCP_TXB0DLC, buf_size);

    send_buffer();

    // Send tx
    set_register(MCP_TXB0CTRL, 0b00001000);

    // Checking if tx has transmitted and if there are any errors
    read_register(MCP_TXB0CTRL, 0b00011000);

    bool success = mcp2515_driver_send_msg_buffer(can_id, 0, buf_size, tx_buf);
    TEST_ASSERT_FALSE(success);
}
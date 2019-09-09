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
    spi_driver_cs_low_Expect();
    spi_driver_exchange_ExpectAndReturn(MCP_RESET, param[0]);
    spi_driver_cs_high_Expect();

    spi_driver_cs_low_Expect();
    spi_driver_exchange_ExpectAndReturn(MCP_READ, param[1]);
    spi_driver_exchange_ExpectAndReturn(MCP_CANSTAT, param[2]);
    spi_driver_exchange_ExpectAndReturn(0, param[3]);
    spi_driver_cs_high_Expect();

    // return mcp2515_driver_reset();
}

static void set_register(uint8_t address, uint8_t value)
{
    spi_driver_cs_low_Expect();
    spi_driver_exchange_ExpectAndReturn(MCP_WRITE, 0);
    spi_driver_exchange_ExpectAndReturn(address, 0);
    spi_driver_exchange_ExpectAndReturn(value, 0);
    spi_driver_cs_high_Expect();
}

static void read_register(uint8_t address, uint8_t expected)
{
    spi_driver_cs_low_Expect();
    spi_driver_exchange_ExpectAndReturn(MCP_READ, 0);
    spi_driver_exchange_ExpectAndReturn(address, 0);
    spi_driver_exchange_ExpectAndReturn(0, expected);
    spi_driver_cs_high_Expect();
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

static void init_rx1(void)
{
    set_register(MCP_RXB0CTRL, 0b01100000);
    set_register(MCP_RXB1CTRL, 0b01100000);
    // TODO See if the register below needs setting or not
    // set_register(MCP_BFPCTRL, 0);
}

static void mcp2515_rx0_is_full_expect(uint8_t status)
{
    spi_driver_cs_low_Expect();
    spi_driver_exchange_ExpectAndReturn(MCP_READ_STATUS, 0);
    spi_driver_exchange_ExpectAndReturn(0, status);
    spi_driver_cs_high_Expect();
}

static void mcp2515_read_rx_message_expected(uint8_t * p_id, uint8_t len,
                                             uint8_t * p_data)
{
    spi_driver_cs_low_Expect();

    spi_driver_exchange_ExpectAndReturn(MCP_READ_BUF_RX0, 0);

    // ID buffers
    spi_driver_exchange_ExpectAndReturn(0, p_id[0]);
    spi_driver_exchange_ExpectAndReturn(0, p_id[1]);
    spi_driver_exchange_ExpectAndReturn(0, p_id[2]);
    spi_driver_exchange_ExpectAndReturn(0, p_id[3]);

    // Msg length
    spi_driver_exchange_ExpectAndReturn(0, len);

    // Read msg
    spi_driver_exchange_ExpectAndReturn(0, p_data[0]);
    spi_driver_exchange_ExpectAndReturn(0, p_data[1]);
    spi_driver_exchange_ExpectAndReturn(0, p_data[2]);
    spi_driver_exchange_ExpectAndReturn(0, p_data[3]);
    spi_driver_exchange_ExpectAndReturn(0, p_data[4]);
    spi_driver_exchange_ExpectAndReturn(0, p_data[5]);
    spi_driver_exchange_ExpectAndReturn(0, p_data[6]);
    spi_driver_exchange_ExpectAndReturn(0, p_data[7]);

    spi_driver_cs_high_Expect();
}

void setUp(void)
{
    spi_driver_cs_high_Expect();
    reset(reset_param);

    set_register(MCP_CNF1, MCP_16MHz_1000kBPS_CFG1);
    set_register(MCP_CNF2, MCP_16MHz_1000kBPS_CFG2);
    set_register(MCP_CNF3, MCP_16MHz_1000kBPS_CFG3);

    init_rx1();

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

void test_tx_send_message_size_is_zero(void)
{
    bool success = mcp2515_driver_send_msg_buffer(can_id, 0, 0, tx_buf);
    TEST_ASSERT_FALSE(success);
}

void test_tx_send_message_size_is_greater_than_8(void)
{
    bool success = mcp2515_driver_send_msg_buffer(can_id, 0, 9, tx_buf);
    TEST_ASSERT_FALSE(success);
}

/*******************************************************************************
 * Receive messages
 *******************************************************************************/

void test_rx0_is_full_returns_true_when_rx0IF_is_set(void)
{
    mcp2515_rx0_is_full_expect(0b00000001);

    bool status = mcp2515_rx0_is_full();
    TEST_ASSERT(status);
}

void test_rx0_is_full_returns_false_when_rx0IF_is_clear(void)
{
    mcp2515_rx0_is_full_expect(0b00000000);
    bool status = mcp2515_rx0_is_full();
    TEST_ASSERT_FALSE(status);
}

void test_rx0_read_can_buffer_id_is_read_correctly(void)
{
    uint8_t id          = 0;
    uint8_t len         = 0;
    uint8_t read_buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t p_id[]      = {0xFF, 0xE0, 0, 0};

    mcp2515_read_rx_message_expected(p_id, 8, read_buf);

    mcp2515_driver_read_can_message(&id, &len, read_buf);
    TEST_ASSERT_EQUAL_HEX8(0x7FF, id);
}
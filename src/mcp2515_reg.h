/**
 * @file        mcp2515_reg.h
 * @author      Steven Daglish
 * @brief
 * @version     0.1
 * @date        02 September 2019
 *
 */

#ifndef _MCP2515_REG_H
#define _MCP2515_REG_H

#define MCP_MAX_STD_BUF_SIZE    8

#define MCP_RESET               0b11000000
#define MCP_READ                0b00000011
#define MCP_WRITE               0b00000010
#define MCP_READ_STATUS         0b10100000
#define MCP_READ_BUF_RX0        0b10010000      // Note, this value is incorrect in the datasheet

#define MCP_CANSTAT 0x0E
#define MCP_CANCTRL 0x0F
#define MCP_CNF3 0x28
#define MCP_CNF2 0x29
#define MCP_CNF1 0x2A

#define MCP_16MHz_5kBPS_CFG1 0x3F
#define MCP_16MHz_5kBPS_CFG2 0xFF
#define MCP_16MHz_5kBPS_CFG3 0x87

#define MCP_16MHz_1000kBPS_CFG1 0x00
#define MCP_16MHz_1000kBPS_CFG2 0xD0
#define MCP_16MHz_1000kBPS_CFG3 0x82

#define MCP_16MHZ 1
#define MCP_8MHZ 2

#define CAN_5KBPS 1
#define CAN_10KBPS 2
#define CAN_20KBPS 3
#define CAN_25KBPS 4
#define CAN_31K25BPS 5
#define CAN_33KBPS 6
#define CAN_40KBPS 7
#define CAN_50KBPS 8
#define CAN_80KBPS 9
#define CAN_83K3BPS 10
#define CAN_95KBPS 11
#define CAN_100KBPS 12
#define CAN_125KBPS 13
#define CAN_200KBPS 14
#define CAN_250KBPS 15
#define CAN_500KBPS 16
#define CAN_666KBPS 17
#define CAN_1000KBPS 18

#define MCP_TXB0CTRL 0x30
#define MCP_TXB0SIDH 0x31
#define MCP_TXB0SIDL 0x32
#define MCP_TXB0EID8 0x33
#define MCP_TXB0EID0 0x34
#define MCP_TXB0DLC 0x35
#define MCP_TXB0D0 0x36
#define MCP_TXB0D1 0x37
#define MCP_TXB0D2 0x38
#define MCP_TXB0D3 0x39
#define MCP_TXB0D4 0x3A
#define MCP_TXB0D5 0x3B
#define MCP_TXB0D6 0x3C
#define MCP_TXB0D7 0x3D

#define MCP_TXB1CTRL 0x40
#define MCP_TXB1SIDH 0x41
#define MCP_TXB2CTRL 0x50
#define MCP_TXB2SIDH 0x51
#define MCP_RXB0CTRL 0x60
#define MCP_RXB0SIDH 0x61
#define MCP_RXB1CTRL 0x70
#define MCP_RXB1SIDH 0x71
#define MCP_BFPCTRL 0x0C

#define MCP_CANINTF 0x2C

#endif // _MCP2551_REG_H

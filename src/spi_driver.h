/**
 * @file        spi_driver.h
 * @author      Steven Daglish
 * @brief       Mock file for spi_driver
 * @version     0.1
 * @date        03 September 2019
 *
 * @copyright Copyright (c) 2019
 *
 */

#ifndef _SPI_DRIVER_H
#define _SPI_DRIVER_H

void spi_driver_init(void);
uint8_t spi_driver_exchange(uint8_t data);
void spi_driver_cs_low(void);
void spi_driver_cs_high(void);

#endif // _SPI_DRIVER_H
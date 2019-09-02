#ifndef _SPI_DRIVER_H
#define _SPI_DRIVER_H

void spi_driver_init(void);
uint8_t spi_driver_exchange(uint8_t data);

#endif // _SPI_DRIVER_H
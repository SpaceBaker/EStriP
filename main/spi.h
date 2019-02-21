#include <driver/spi_master.h>
#include <string.h>


void SPI_init(spi_device_handle_t *device_handle, const int mosiPin, const int clkPin, const int clkSpeed);

void SPI_txByte(spi_device_handle_t device_handle, const uint8_t data);

void SPI_transmit(spi_device_handle_t device_handle, const void *data, int size);
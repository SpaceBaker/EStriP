#include "spi.h"


spi_bus_config_t buscfg;
spi_device_interface_config_t devcfg;


void SPI_init(spi_device_handle_t *device_handle, int mosiPin, int clkPin, int clkSpeed)
{
    esp_err_t ret;

    buscfg.miso_io_num        =   -1;                            //not used
    buscfg.mosi_io_num        =   mosiPin;
    buscfg.sclk_io_num        =   clkPin;
    buscfg.quadwp_io_num      =   -1;                            //not used
    buscfg.quadhd_io_num      =   -1;                            //not used
    buscfg.max_transfer_sz    =   0;                              //default to 4094 bytes if 0

    ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);                                       //terminate the program in case the code is not ESP_OK
    
    devcfg.clock_speed_hz =   clkSpeed;                           //this is for 11.43MHz. adjust as needed
    devcfg.mode           =   0;
    devcfg.spics_io_num   =   -1;                                 //no cs pin
    devcfg.queue_size     =   1;                                  //how many transactions will be queued at once
    devcfg.flags          =   SPI_DEVICE_NO_DUMMY;

    ret = spi_bus_add_device(VSPI_HOST, &devcfg, device_handle);
    ESP_ERROR_CHECK(ret);                                       //terminate the program in case the code is not ESP_OK
}

void SPI_txByte(spi_device_handle_t device_handle, const uint8_t data)
{
    esp_err_t ret;
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &data;
    t.rx_buffer = NULL;

    ret = spi_device_transmit(device_handle, &t);
    assert(ret == ESP_OK);
}

void SPI_transmit(spi_device_handle_t device_handle, const void *data, int size)
{
    esp_err_t ret;
    spi_transaction_t t;

    if (size == 0)
        return;

    memset(&t, 0, sizeof(t));
    t.length = size * 8;
    t.tx_buffer = data;
    t.rx_buffer = NULL;

    ret = spi_device_transmit(device_handle, &t);
    assert(ret == ESP_OK);
}
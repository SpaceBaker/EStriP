#include "sdkconfig.h"
#include "driver/spi_master.h"


#define MOSI_PIN 13
#define SCLK_PIN 14


void app_main()
{
    esp_err_t ret;                                      //return value variable
    spi_device_handle_t apa102_spi;
    spi_bus_config_t buscfg={
        .miso_io_num=-1,                                //not used
        .mosi_io_num=MOSI_PIN,
        .sclk_io_num=SCLK_PIN,
        .quadwp_io_num=-1,                              //not used
        .quadhd_io_num=-1,                              //not used
        .max_transfer_sz=0                              //default to 4094 bytes if 0
    };
    if(spicommon_dma_chan_claim(1))
        spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    else
        printf("DMA channel 1 cannot be claimed");
}
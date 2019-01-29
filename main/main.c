#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sdkconfig.h>
#include <driver/spi_master.h>


#define MOSI_PIN 23
#define SCLK_PIN 18

const uint8_t START_FRAME[] = {0x00,0x00,0x00,0x00};
/* LED_FRAM [GLOBAL]  [BLUE]  [GREEN] [RED]
        [111 + 5 bit] [8 bit] [8 bit] [8 bit]
*/
const uint8_t BLUE[] = {0xff, 0xff, 0x00, 0x00};
const uint8_t GREEN[] = {0xff, 0x00, 0xff, 0x00};
const uint8_t RED[] = {0xff, 0x00, 0x00, 0xff};
const uint8_t OFF[] = {0xff, 0x00, 0x00, 0x00};
const uint8_t WHITE[] = {0xff, 0xff, 0xff, 0xff};
const uint8_t PURPLE[] = {0xff, 204, 0x00, 125};

void SPI_txByte(spi_device_handle_t spi, const uint8_t *data, int len);

void app_main()
{
    esp_err_t ret;                                             //error check variable
    spi_device_handle_t apa102_spi;
    spi_bus_config_t buscfg={
        .miso_io_num        =   -1,                            //not used
        .mosi_io_num        =   MOSI_PIN,
        .sclk_io_num        =   SCLK_PIN,
        .quadwp_io_num      =   -1,                            //not used
        .quadhd_io_num      =   -1,                            //not used
        .max_transfer_sz    =   0                              //default to 4094 bytes if 0
    };
    
    spi_device_interface_config_t devcfg={
        .clock_speed_hz =   SPI_MASTER_FREQ_8M,                //this is for 11.43MHz. adjust as needed
        .mode           =   0,
        .spics_io_num   =   -1,                                 //no cs pin
        .queue_size     =   3,                                  //how many transactions will be queued at once
        .flags          =   SPI_DEVICE_NO_DUMMY
    };

    ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);                                       //terminate the program in case the code is not ESP_OK

    ret = spi_bus_add_device(VSPI_HOST, &devcfg, &apa102_spi);
    ESP_ERROR_CHECK(ret);                                       //terminate the program in case the code is not ESP_OK

    /*  CLEAR ALL LED   */
    SPI_txByte(apa102_spi, START_FRAME, 4);
    for (int i = 0; i < 60; i++)
        SPI_txByte(apa102_spi, OFF, 4);
    /********************/

    /*  CHASER MODE     */  
    while(true){
        SPI_txByte(apa102_spi, START_FRAME, 4);
        for (int i = 0; i < 60; i++){
            SPI_txByte(apa102_spi, PURPLE, 4);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        SPI_txByte(apa102_spi, START_FRAME, 4);
        for (int i = 0; i < 60; i++){
            SPI_txByte(apa102_spi, OFF, 4);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
    /********************/

    /** What color blending should look like
     * 
     * [255,255,255]
     * 
     * [---,255,255]
     * [000,255,255]
     * 
     * [000,---,255]
     * [000,000,255]
     * 
     * [+++,000,255]
     * [255,000,255]
     * 
     * [255,000,---]
     * [255,000,000]
     * 
     * [255,+++,000]
     * [255,255,000]
     * 
     * [---,255,000]
     * [000,255,000]
     * 
     * [+++,255,+++]
     * 
     */


/*  Color blending (Still in progress... blink to yellow after green transition)
 * 
 * 
    uint8_t color[] = {0xff,255,255,255};
    uint8_t b = 255;
    uint8_t g = 255;
    uint8_t r = 255;
    int led = 0;

    //[255,255,255]
    SPI_txByte(apa102_spi, START_FRAME, 4);
        for (led = 0; led < 60; led++)
            SPI_txByte(apa102_spi, WHITE, 4);

    while(true)
    {
        // [---,255,255]
        // [000,255,255]
        for (; b > 0; --b){
            color[1] = b;
            vTaskDelay(18 / portTICK_PERIOD_MS);
            SPI_txByte(apa102_spi, START_FRAME, 4);
            for (led = 0; led < 60; led++)
                SPI_txByte(apa102_spi, color, 4);
        }

        // [000,---,255]
        // [000,000,255]
        for (; g > 0; --g){
            color[2] = g;
            vTaskDelay(18 / portTICK_PERIOD_MS);
            SPI_txByte(apa102_spi, START_FRAME, 4);
            for (led = 0; led < 60; led++)
                SPI_txByte(apa102_spi, color, 4);
        }

        // [+++,000,255]
        // [255,000,255]
        for (; b < 255; ++b){
            color[1] = b;
            vTaskDelay(18 / portTICK_PERIOD_MS);
            SPI_txByte(apa102_spi, START_FRAME, 4);
            for (led = 0; led < 60; led++)
                SPI_txByte(apa102_spi, color, 4);
        }

        // [255,000,---]
        // [255,000,000]
        for (; r > 0; --r){
            color[3] = r;
            vTaskDelay(18 / portTICK_PERIOD_MS);
            SPI_txByte(apa102_spi, START_FRAME, 4);
            for (led = 0; led < 60; led++)
                SPI_txByte(apa102_spi, color, 4);
        }

        // [255,+++,000]
        // [255,255,000]
        for (; g < 255; ++g){
            color[2] = g;
            vTaskDelay(18 / portTICK_PERIOD_MS);
            SPI_txByte(apa102_spi, START_FRAME, 4);
            for (led = 0; led < 60; led++)
                SPI_txByte(apa102_spi, color, 4);
        }

        // [---,255,000]
        // [000,255,000]
        for (; b > 0; --b){
            color[1] = b;
            vTaskDelay(18 / portTICK_PERIOD_MS);
            SPI_txByte(apa102_spi, START_FRAME, 4);
            for (led = 0; led < 60; led++)
                SPI_txByte(apa102_spi, color, 4);
        }

        // [+++,255,+++]
        for (b = 0; b < 255; ++b){
            color[2] = b;
            r++;
            color[3] = r;
            vTaskDelay(18 / portTICK_PERIOD_MS);
            SPI_txByte(apa102_spi, START_FRAME, 4);
            for (led = 0; led < 60; led++)
                SPI_txByte(apa102_spi, color, 4);
        }
    }
*/
}


//Send data to APA102. Uses spi_device_transmit, which waits until the transfer is complete.
void SPI_txByte(spi_device_handle_t spi, const uint8_t *data, int len)
{
    esp_err_t ret;
    spi_transaction_t t;
    if (len == 0) return;             //no need to send anything
    memset(&t, 0, sizeof(t));         //Zero out the transaction
    t.length = len*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer = data;               //Data
    t.rx_buffer = NULL;
    ret = spi_device_transmit(spi, &t);  //Transmit!
    assert(ret == ESP_OK);            //Should have had no issues.
}
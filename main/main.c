#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sdkconfig.h>
#include "spi.h"


#define MOSI_PIN 23
#define SCLK_PIN 18
#define LED_NUM  60

DMA_ATTR uint8_t led[LED_NUM][4];

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

spi_device_handle_t apa102_spi;

void app_main()
{
    //spi_device_handle_t apa102_spi = {0};

    SPI_init(&apa102_spi, MOSI_PIN, SCLK_PIN, SPI_MASTER_FREQ_20M);

    /*  CLEAR ALL LED   */
    for (int i = LED_NUM; i > 0 ; --i){
        led[i][0] = 0xFF;
        led[i][1] = 0x00;
        led[i][2] = 0x00;
        led[i][3] = 0x00;
    }
    SPI_transmit(apa102_spi, START_FRAME, 4);
    SPI_transmit(apa102_spi, led, LED_NUM*4);
    /********************/

    /*  CHASER MODE     */  
    while(true){
        SPI_transmit(apa102_spi, START_FRAME, 4);
        for (int i = 0; i < LED_NUM; i++){
            SPI_transmit(apa102_spi, PURPLE, 4);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        SPI_transmit(apa102_spi, START_FRAME, 4);
        for (int i = 0; i < LED_NUM; i++){
            SPI_transmit(apa102_spi, OFF, 4);
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
    /********************/
}
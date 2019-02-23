#include "apa102.h"

DMA_ATTR uint8_t led[LED_NUM][4];
const uint8_t START_FRAME[] = {0x00,0x00,0x00,0x00};

/***    APA102 functions    ***/
void APA102_init()
{
    SPI_init(&apa102_spi, MOSI_PIN, SCLK_PIN, SPI_SPEED);

    /*  CLEAR ALL LED   */
    APA102_setColor(0xFF,0x00,0x00,0x00,-1,0);
    APA102_show();
}

/**
 * @brief Transmit color values to all led.
 */
void APA102_show()
{
    SPI_transmit(apa102_spi, START_FRAME, 4);
    SPI_transmit(apa102_spi, led, LED_NUM*4);
}

void APA102_setColor(uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue, int fromLed, int toLed)
{
    brightness |= 0xE0;

    if (fromLed >= LED_NUM)
        fromLed = LED_NUM-1;
    else if (fromLed < 0)
    {
        for(int i = 0; i < LED_NUM; i++)
        {
            led[i][0] = brightness;
            led[i][1] = red;
            led[i][2] = green;
            led[i][3] = blue;
        }
        return;
    }

    if (toLed >= LED_NUM)
        toLed = LED_NUM-1;
    
    do{
        led[fromLed][0] = brightness;
        led[fromLed][1] = red;
        led[fromLed][2] = green;
        led[fromLed][3] = blue;
        fromLed++;
    }while(fromLed <= toLed);
}

void APA102_simpleColorTransition_task(void *pvParam)
{
    enum states{
        REDTOGREEN,
        REDTOBLUE,
        GREENTORED,
        GREENTOBLUE,
        BLUETORED,
        BLUETOGREEN
    }state = REDTOGREEN;
    
    const uint8_t MAXVAL = 255;
    uint8_t r = 255;
    uint8_t g = 0;
    uint8_t b = 0;

    APA102_setColor(0xFF,r,0,0,-1,0);
    APA102_show();

    while(true)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
        switch (state)
        {
            case REDTOGREEN:
                if(g < MAXVAL)
                    g++;
                else
                    r--;
                APA102_setColor(0xFF,r,g,b,-1,0);
                APA102_show();
                if(g == MAXVAL && r == 0)
                    state = GREENTOBLUE;
                break;

            case REDTOBLUE:
                /* code */
                break;

            case GREENTORED:
                /* code */
                break;

            case GREENTOBLUE:
                if(b < MAXVAL)
                    b++;
                else
                    g--;
                APA102_setColor(0xFF,r,g,b,-1,0);
                APA102_show();
                if(b == MAXVAL && g == 0)
                    state = BLUETORED;
                break;

            case BLUETORED:
                if(r < MAXVAL)
                    r++;
                else
                    b--;
                APA102_setColor(0xFF,r,g,b,-1,0);
                APA102_show();
                if(r == MAXVAL && b == 0)
                    state = REDTOGREEN;
                break;

            case BLUETOGREEN:
                /* code */
                break;
        
            default:
                APA102_setColor(0xFF,0xFF,0xFF,0xFF,-1,0);
                APA102_show();
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                APA102_setColor(0xFF,0,0,0,-1,0);
                APA102_show();
                vTaskDelay(5000 / portTICK_PERIOD_MS);
                break;
        }
    }
    vTaskDelete(NULL);
}



/***    SPI functions   ***/
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

void SPI_transmitByte(spi_device_handle_t device_handle, const uint8_t data)
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
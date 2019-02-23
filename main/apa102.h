#include <freertos/FreeRTOS.h>
#include <driver/spi_master.h>
#include <freertos/task.h>
#include <sdkconfig.h>
#include <string.h>



/************* SPI variables *************/
#define MOSI_PIN    23
#define SCLK_PIN    18
#define SPI_SPEED   SPI_MASTER_FREQ_20M
spi_device_handle_t apa102_spi;
spi_bus_config_t buscfg;
spi_device_interface_config_t devcfg;


/************* APA102 variables *************/
#define LED_NUM     62
/*
const uint8_t OFF[] = {0xff, 0x00, 0x00, 0x00};
const uint8_t BLUE[] = {0xff, 0xff, 0x00, 0x00};
const uint8_t GREEN[] = {0xff, 0x00, 0xff, 0x00};
const uint8_t RED[] = {0xff, 0x00, 0x00, 0xff};
const uint8_t WHITE[] = {0xff, 0xff, 0xff, 0xff};
*/


/************* APA102 FUNCTIONS *************/
void APA102_init(void);

void APA102_show(void);

/**
 * @fromLed -1 for all LED
 */
void APA102_setColor(uint8_t brightness, uint8_t red, uint8_t green, uint8_t blue, int fromLed, int toLed);

void APA102_simpleColorTransition_task(void *pvParam);


/************* SPI FUNCTIONS *************/
void SPI_init(spi_device_handle_t *device_handle, const int mosiPin, const int clkPin, const int clkSpeed);

void SPI_txByte(spi_device_handle_t device_handle, const uint8_t data);

void SPI_transmit(spi_device_handle_t device_handle, const void *data, int size);
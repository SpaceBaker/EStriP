#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sdkconfig.h>
#include <string.h>
#include "apa102.h"


/****************************************
 *              TODO                    *
 * Define all possible modes and what   *
 *  they do.                            *
 *                                      *
 * Code all, or almost all, those modes.*
 *                                      *
 * Using wifi, think of a communication *
 *  protocol to handle mode changing.   *
 *                                      *
 * Try receiving data from wifi.        *
 *                                      *
 * Implement communication protocol     *
 *  with wifi.                          *
 ****************************************/



void app_main()
{
    APA102_init();

    xTaskCreate(APA102_simpleColorTransition_task,"simpleColorTransition_task",configMINIMAL_STACK_SIZE,NULL,1,NULL);

    //APA102_setColor(0xFF,0xFF,0,0,-1,0);
 /*   while (true)
    {


    *  CHASER MODE  *
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
    ********************
*/
}
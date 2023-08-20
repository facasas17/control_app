/*
 * Copyright (c) 2023, Fabiola de las Casas Escardo <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Version: 1.0
 * 
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "display.h"

#include "protocol.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DISPLAY_TASK_DELAY      100

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
QueueHandle_t sensorData_queue;
easyNex_t myNextion;
/*******************************************************************************
 * Code - private
 ******************************************************************************/

/*******************************************************************************
 * Code - public
 ******************************************************************************/
void displayTask(void *arg)
{
    sensorData_queue = xQueueCreate(10, sizeof(protocol_frame_t));
    
    if(sensorData_queue == NULL)
    {
        while(1);
    }

    EasyNex_begin(&myNextion, HMI_PORT, HMI_BAUDRATE);

    while (1) 
    {
        EasyNex_NextionListen(&myNextion);  //Lo uso para limpiar el buffer de la UART
        // xQueueReceive(temperatureQueue, &(saunaHMI.temperature), 0);

        //refreshScreen();
        // Serial.println("Estado pantalla:");
        // Serial.printf("Pagina actual: %d \n", myNex.currentPageId);
        // Serial.printf("Modo de luz: %d \n", (uint32_t)saunaHMI.lights);

        vTaskDelay(20 / portTICK_PERIOD_MS);



        vTaskDelay(DISPLAY_TASK_DELAY / portTICK_PERIOD_MS);
    }
}
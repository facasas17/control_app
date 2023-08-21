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
#define DISPLAY_TASK_DELAY  100

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
easyNex_t myNextion;

/*******************************************************************************
 * Code - private
 ******************************************************************************/

/*******************************************************************************
 * Code - public
 ******************************************************************************/
void displayTask(void *arg)
{
    protocol_frame_t frame;

    EasyNex_begin(&myNextion, HMI_PORT, HMI_BAUDRATE);

    while (1) 
    {
        EasyNex_NextionListen(&myNextion);  //Lo uso para limpiar el buffer de la UART
        //xQueueReceive(tempData_queue, &frame, 0);

        //refreshScreen();

        vTaskDelay(DISPLAY_TASK_DELAY / portTICK_PERIOD_MS);
    }
}
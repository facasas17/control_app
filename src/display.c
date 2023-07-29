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

/*******************************************************************************
 * Code - private
 ******************************************************************************/

/*******************************************************************************
 * Code - public
 ******************************************************************************/
void displayTask(void *arg)
{
    while (1) 
    {
        vTaskDelay(DISPLAY_TASK_DELAY / portTICK_PERIOD_MS);
    }
}
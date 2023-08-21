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
static void refreshScreen(void);

static void refreshScreenInicial(void);
static void refreshScreenSilo1(void);
static void refreshScreenSilo2(void);
static void refreshScreenSilo3(void);
static void refreshScreenSilo4(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
easyNex_t myNextion;

extern QueueHandle_t humData_queue;
extern QueueHandle_t tempData_queue;
/*******************************************************************************
 * Code - private
 ******************************************************************************/
static void refreshScreen(void)
{
    switch (myNextion.currentPageId)
    {
        case 0:
            refreshScreenInicial();
            break;
        case 1:
            refreshScreenSilo1();
            break;
        case 2:
            refreshScreenSilo2();
            break;
        case 3:
            refreshScreenSilo3();
            break;
        case 4:
            refreshScreenSilo4();
            break;

        default:
            break;
    }
}

static void refreshScreenInicial(void)
{

}

static void refreshScreenSilo1(void)
{

}

static void refreshScreenSilo2(void)
{

}

static void refreshScreenSilo3(void)
{

}

static void refreshScreenSilo4(void)
{

}

/*******************************************************************************
 * Code - public
 ******************************************************************************/
void displayTask(void *arg)
{
    display_data_t temperatura;
    display_data_t humedad;

    EasyNex_begin(&myNextion, HMI_PORT, HMI_BAUDRATE);

    while (1) 
    {
        EasyNex_NextionListen(&myNextion);  //Lo uso para limpiar el buffer de la UART

        xQueueReceive(tempData_queue, &temperatura, 0);

        xQueueReceive(humData_queue, &humedad, 0);

        refreshScreen();

        vTaskDelay(DISPLAY_TASK_DELAY / portTICK_PERIOD_MS);
    }
}
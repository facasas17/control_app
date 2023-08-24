/*
 * Copyright (c) 2023, Fabiola de las Casas Escardo <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Version: 1.0
 * 
 */
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "nodes.h"
#include "stdio.h"
#include "esp_log.h"
static const char *TAG = "NODE";
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NODE_TASK_DELAY     100

#define NODE_UART_PORT      UART_NUM_0
#define NODE_UART_BAUDRATE  9600    //4800
#define NODE_UART_BUF_SIZE  256
#define NODE_BUF_SIZE       (PROTOCOL_SIZE * 8)      // Tamaño del protocolo

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hardware_config(void);
static void calcular_PromedioNivel(uint16_t *temperatura, uint16_t *hum, protocol_frame_t node1, 
                                                protocol_frame_t node2, protocol_frame_t node3);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t manager_address = 0x00;

display_data_t temp_silo[CANT_SILOS][CANT_NIVELES];
display_data_t hum_silo[CANT_SILOS][CANT_NIVELES];

QueueHandle_t node_uart_queue;
QueueHandle_t humData_queue;
QueueHandle_t tempData_queue;

/*******************************************************************************
 * Code - private
 ******************************************************************************/
static void hardware_config(void)
{
    UART_Config(NODE_UART_PORT, NODE_UART_BAUDRATE, NODE_UART_BUF_SIZE, &node_uart_queue, manager_address);
    
    RS485_ConfigGPIO();
}

static void calcular_PromedioNivel(uint16_t *temperatura, uint16_t *hum, protocol_frame_t node1, 
                                                protocol_frame_t node2, protocol_frame_t node3)
{
    uint32_t num1, num2, num3;

    uint32_t mascara_temp = 0xFFFF0000;
    
    num1 = (uint16_t)(node1.payload & mascara_temp);
    num2 = (uint16_t)(node2.payload & mascara_temp);
    num3 = (uint16_t)(node3.payload & mascara_temp);

    *temperatura = (num1 + num2 + num3)/3;      

    num1 = (uint16_t)(node1.payload >> 16);
    num2 = (uint16_t)(node2.payload >> 16);
    num3 = (uint16_t)(node3.payload >> 16);

    *hum = (num1 + num2 + num3)/3;      
}

/*******************************************************************************
 * Code - public
 ******************************************************************************/
void nodeManagerTask(void *arg)
{
    uint8_t i;
    node_number_t node;
    uint16_t len_data = 0;
    uart_event_t event;
    int uart_read_flag = 0;

    protocol_frame_t frame_send;
    protocol_frame_t frame_receive[TOTAL_NODES];
    char data_send[NODE_BUF_SIZE];
    char data_receive[NODE_BUF_SIZE];

    hardware_config();
    
    humData_queue = xQueueCreate(10, sizeof(display_data_t));
    if(humData_queue == NULL)
    {
        while(1);
    }

    tempData_queue = xQueueCreate(10, sizeof(display_data_t));   
    if(tempData_queue == NULL)
    {
        while(1);
    }

    while (1) 
    {
        ESP_LOGI(TAG, "Esto es una prueba\n");
        for(node=NODE_01; node<TOTAL_NODES; node++)
        {
            RS485_EnableSendData();
            protocol_setFrame(&frame_send, node, ACTION_ONE, FLAG_OK, 0, 0); 

            len_data = protocol_buildFrame(data_send, &frame_send);

            if( UART_OK == UART_WaitTX(NODE_UART_PORT))
            {
                UART_SendData(NODE_UART_PORT, data_send, len_data);
            }

            RS485_EnableReceiveData();

            if (xQueueReceive(node_uart_queue, (void *)&event, 5000/ portTICK_PERIOD_MS))
            {      
                if (event.type == UART_PATTERN_DET)
                {
                    uart_read_flag = UART_ReadData(NODE_UART_PORT, data_receive, PROTOCOL_SIZE - 1);
                }

                if(UART_FAIL != uart_read_flag)
                {
                    protocol_readFrame(data_receive, &frame_receive[node]);
                }

                memset(data_receive, 0, NODE_BUF_SIZE);      // Reset buffer
            }
        }

        node = 0;

        for(i=0; i<CANT_NIVELES ; i++)
        {
            temp_silo[CANT_SILOS][i].nivel = i+1;
            hum_silo[CANT_SILOS][i].nivel = i+1;
            calcular_PromedioNivel(&temp_silo[CANT_SILOS][i].data, &hum_silo[CANT_SILOS][i].data,
                                    frame_receive[node], frame_receive[node+CANT_NIVELES], frame_receive[node+CANT_NIVELES*2]);
            ++node;

            xQueueSendToBack( humData_queue, ( void * ) &temp_silo[CANT_SILOS][i], ( TickType_t ) 1000 );
            xQueueSendToBack( tempData_queue, ( void * ) &hum_silo[CANT_SILOS][i], ( TickType_t ) 1000 );
        }
        vTaskDelay(NODE_TASK_DELAY / portTICK_PERIOD_MS);
    }
}
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
#include "nodes.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NODE_TASK_DELAY     10000

#define NODE_UART_PORT      UART_NUM_0
#define NODE_UART_BAUDRATE  9600    //4800
#define NODE_BUF_SIZE       (PROTOCOL_SIZE * 8)      // Tamaño del protocolo

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hardware_config(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t manager_address = 0x00;

QueueHandle_t node_uart_queue;

/*******************************************************************************
 * Code - private
 ******************************************************************************/
static void hardware_config(void)
{
    UART_Config(NODE_UART_PORT, NODE_UART_BAUDRATE, NODE_BUF_SIZE, &node_uart_queue, manager_address);
    
    RS485_ConfigGPIO();
}

/*******************************************************************************
 * Code - public
 ******************************************************************************/

void nodeManagerTask(void *arg)
{
    node_number_t node;
    uint16_t len_data = 0;
    uart_event_t event;
    int uart_read_flag;

    protocol_frame_t frame_send;
    protocol_frame_t frame_receive;
    char data_send[NODE_BUF_SIZE];
    char data_receive[NODE_BUF_SIZE];
    
    hardware_config();

    while (1) 
    {
        
        for(node=NODE_01; node<TOTAL_NODES; node++)
        {
            RS485_EnableSendData();
            protocol_setFrame(&frame_send, node, ACTION_ONE, FLAG_OK, 0, 0); 

            len_data = protocol_buildFrame(data_send, &frame_send);

            if( UART_OK == UART_WaitTX(NODE_UART_PORT))
            {
                UART_SendData(NODE_UART_PORT, data_send, len_data);
            }


        if (xQueueReceive(node_uart_queue, (void *)&event, 5000/ portTICK_PERIOD_MS))
        {      
            if (event.type == UART_PATTERN_DET)
            {
                uart_read_flag = UART_ReadData(NODE_UART_PORT, data_receive, PROTOCOL_SIZE - 1);
            }

            if(UART_FAIL != uart_read_flag)
            {
                protocol_readFrame(data_receive, &frame_receive);
            }
            

            
        }





        }
        

        vTaskDelay(NODE_TASK_DELAY / portTICK_PERIOD_MS);
    }
}
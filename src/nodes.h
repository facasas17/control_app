/*
 * Copyright (c) 2023, Fabiola de las Casas Escardo <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Version: 1.0
 * 
 */

#ifndef NODES_NODES_H_
#define NODES_NODES_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "uartDriver.h"
#include "protocol.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum{
    NODE_01 = 0x01,
    NODE_02 = 0x02,
    NODE_03 = 0x03,
    NODE_04 = 0x04,
    NODE_05 = 0x05,
    NODE_06 = 0x06,
    NODE_07 = 0x07,
    NODE_08 = 0x08,
    NODE_09 = 0x09,
    TOTAL_NODES
}node_number_t;

/*******************************************************************************
 * API
 ******************************************************************************/
void nodeManagerTask(void *arg);

#endif /* NODES_NODESS_H_ */
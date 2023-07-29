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


/*******************************************************************************
 * API
 ******************************************************************************/
void nodeCommTask(void *arg);

#endif /* NODES_NODESS_H_ */
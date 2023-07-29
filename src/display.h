/*
 * Copyright (c) 2023, Fabiola de las Casas Escardo <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Version: 1.0
 * 
 */

#ifndef DISPLAY_DISPLAY_H_
#define DISPLAY_DISPLAY_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * API
 ******************************************************************************/
void displayTask(void *arg);

#endif /* DISPLAY_DISPLAY_H_ */
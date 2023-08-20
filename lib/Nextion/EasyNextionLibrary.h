/*!
 * EasyNextionLibrary.h - Easy library for Nextion Displays
 * Copyright (c) 2020 Athanasios Seitanis < seithagta@gmail.com >. 
 * All rights reserved under the library's licence
 * Adapted for ESP-IDF by Fabiola de las Casas < fabioladelascasas@gmail.com >
 */

#ifndef EasyNextionLibrary_h
#define EasyNextionLibrary_h

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "stdio.h"
#include <stdint.h>
#include "driver/uart.h"

/*******************************************************************************
 * Definitions
******************************************************************************/
// Define your UART configurations here
#define UART_NUM UART_NUM_1
#define TX_PIN  GPIO_NUM_17
#define RX_PIN  GPIO_NUM_16

typedef struct {
    // Add the required variables here
    uint8_t uart_port;
    uint32_t baudrate;
    int currentPageId;          //Shows the id of the current page shown on Nextion
    int lastCurrentPageId;       //Stores the value of the previous page shown on Nextion
    uint8_t cmdGroup;           //ONLY for custom commands stores the command group ID 
    uint8_t cmdLength;          //ONLY for custom commands stores the length of the command
} easyNex_t;

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Initialization of nextion structure and Serial port
*/
void EasyNex_begin(easyNex_t* nex, uint8_t port, uint32_t baud);

/**
 * @brief For writing in component's numeric attribute
 * 
 * @param nex Nextion values
 * @param component: objectname.numericAttribute (example: "n0.val"  or "n0.bco".....etc)
 * @param numVal value to write in component's attribute
 * 
 * Syntax: | EasyNex_writeNum(&nex, "n0.val", 765);  | set the value of numeric n0 to 765 | 
 * 
*/
void EasyNex_writeNum(easyNex_t* nex, const char* component, unsigned int numVal);

/**
 * @brief For writing in component's text attributes
 * 
 * @param nex Nextion values
 * @param component: objectname.textAttribute (example: "t0.txt"  or "b0.txt")
 * @param txt string yo write in txt attribute (example: "Hello World")
 * 
 * Syntax: | myObject.writeStr("t0.txt", "Hello World");  |  or  | myObject.writeNum("b0.txt", "Button0"); |
 *         | set the value of textbox t0 to "Hello World" |      | set the text of button b0 to "Button0"  |
 * 
*/
void EasyNex_writeStr(easyNex_t* nex, const char* command, const char* txt);

/**
 * @brief It uses a custom protocol to identify commands from Nextion Touch Events
 *
 * @param nex Nextion values
 * 
 * For advanced users: You can modify the custom protocol to add new group commands.
 * More info on custom protocol: https://www.seithan.com/  and on the documentation of the library
 * WARNING: This function must be called repeatedly to response touch events
 * from Nextion touch panel. Actually, you should place it in your loop function.
*/
void EasyNex_NextionListen(easyNex_t* nex);

/**
 * @brief Read the value of a component's numeric attribute
 * 
 * @param nex Nextion values
 * @param comp component to read attribute from
 * 
 * Syntax: | myObject.readNumber("n0.val"); |  or  | myObject.readNumber("b0.bco");                       |
 *         | read the value of numeric n0   |      | read the color number of the background of button b0 |
 * 
*/
uint32_t EasyNex_readNumber(easyNex_t* nex, const char* comp);

/**
 * @brief Read the value of every components' text attribute from Nextion
 * 
 * @param nex Nextion values
 * @param comp component to read text from  
 *
 * Syntax: String x = myObject.readStr("t0.txt"); // Store to x the value of text box t0
*/
char* EasyNex_readStr(easyNex_t* nex, const char* comp);

/**
 * @brief Read the next byte from the Serial. Main purpose and usage is for the custom commands read
 *          Where we need to read bytes from Serial inside user code
 * Syntax: | myObject.readByte(); |
*/
int EasyNex_readByte(easyNex_t* nex);

void EasyNex_readCommand(void);

#endif
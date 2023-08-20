/*!
 * EasyNextionLibrary.cpp - Easy library for Nextion Displays
 * Copyright (c) 2020 Athanasios Seitanis < seithagta@gmail.com >. 
 * All rights reserved under the library's licence
 * Adapted for ESP-IDF by Fabiola de las Casas < fabioladelascasas@gmail.com >
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
// #include "esp_log.h"
// #include "esp_system.h"
// #include "esp_timer.h"
#include "string.h"

#include "EasyNextionLibrary.h"
#include "callTriggers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
 #define BUFFER_SIZE    128

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void EasyNex_clearSerialBuffer(easyNex_t *nex);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const char* TAG = "EasyNex";

char global_start_char;

uint8_t global_cmdFound;
uint8_t global_cmd;
uint8_t global_len;

/*******************************************************************************
 * Code - Private
 ******************************************************************************/
static void EasyNex_clearSerialBuffer(easyNex_t *nex) 
{
    uint8_t buffer[BUFFER_SIZE];
    size_t length = 0;

    uart_flush_input(nex->uart_port);

    while (uart_read_bytes(nex->uart_port, buffer, sizeof(buffer), 0) > 0) 
    {
        length += sizeof(buffer);
    }
}

/*******************************************************************************
 * Code - Public
 ******************************************************************************/
void EasyNex_begin(easyNex_t *nex, uint8_t port, uint32_t baud) 
{
    nex->baudrate = baud;
    nex->uart_port = port;

    uart_config_t uart_config = {
        .baud_rate = nex->baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };

    uart_param_config(nex->uart_port, &uart_config);
    uart_set_pin(nex->uart_port, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(nex->uart_port, 1024, 0, 0, NULL, 0);

    EasyNex_clearSerialBuffer(nex);
}

void EasyNex_writeNum(easyNex_t* nex, const char* component, unsigned int numVal) 
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%s=%u\xff\xff\xff", component, numVal);
    uart_write_bytes(nex->uart_port, buffer, strlen(buffer));
}

void EasyNex_writeStr(easyNex_t* nex, const char* command, const char* txt) 
{
    char buffer[128];

    if (strcmp(txt, "cmd") == 0) 
    {
        snprintf(buffer, sizeof(buffer), "%s\xff\xff\xff", command);
    } 
    else 
    {
        snprintf(buffer, sizeof(buffer), "%s=\"%s\"\xff\xff\xff", command, txt);
    }
    uart_write_bytes(nex->uart_port, buffer, strlen(buffer));
}

void EasyNex_NextionListen(easyNex_t* nex) 
{
    char _start_char;
    uint8_t _len;
    char buffer[128];
    bool _cmdFound = false;

    if(UART_Available(nex->uart_port) > 2)
    {
        _start_char = uart_read_bytes(nex->uart_port, &_start_char, 1, 100);

        while (_start_char != '#') 
        {
            uart_read_bytes(nex->uart_port, &_start_char, 1, 100);
        }

        if(_start_char == '#')
        {
            _cmdFound = true;
        }
        else 
            _cmdFound = false;

        if (_cmdFound) 
        {
            uart_read_bytes(nex->uart_port, &_len, 1, 100);  // Leo el largo del mensaje

            uart_read_bytes(nex->uart_port, buffer, _len, 100);  //Leo el resto de los bytes
            buffer[_len] = '\0';

            EasyNex_readCommand();
        }
    }
}

uint32_t EasyNex_readNumber(easyNex_t* nex, const char* comp)
{
    // String _comp = comp;
    // bool _endOfCommandFound = false;
    // char _tempChar;

    // _numberValue = 777777;
    // _tmr1 = esp_timer_get_time() / 1000;
    // while (uart_available(nex->uart_port)) {
    //     if ((esp_timer_get_time() / 1000) - _tmr1 > 1000UL) {
    //         _numberValue = 777777;
    //         break;
    //     } else {
    //         NextionListen();
    //     }
    // }

    // _serial_write(nex->uart_port, "get ");
    // _serial_write(nex->uart_port, _comp.c_str());
    // _serial_write(nex->uart_port, "\xff\xff\xff");

    // _tmr1 = esp_timer_get_time() / 1000;
    // while (uart_available(nex->uart_port) < 8) {
    //     if ((esp_timer_get_time() / 1000) - _tmr1 > 400UL) {
    //         _numberValue = 777777;
    //         break;
    //     }
    // }

    // if (uart_available(nex->uart_port) > 7) {
    //     _start_char = uart_read_bytes(nex->uart_port, &_start_char, 1, portMAX_DELAY);
    //     _tmr1 = esp_timer_get_time() / 1000;

    //     while (_start_char != 0x71) {
    //         if (uart_available(nex->uart_port)) {
    //             _start_char = uart_read_bytes(nex->uart_port, &_start_char, 1, portMAX_DELAY);
    //         }

    //         if ((esp_timer_get_time() / 1000) - _tmr1 > 100UL) {
    //             _numberValue = 777777;
    //             break;
    //         }
    //     }

    //     if (_start_char == 0x71) {
    //         for (int i = 0; i < 4; i++) {
    //             _numericBuffer[i] = uart_read_bytes(nex->uart_port, &_tempChar, 1, portMAX_DELAY);
    //         }
    //         int _endBytes = 0;
    //         _tmr1 = esp_timer_get_time() / 1000;

    //         while (!_endOfCommandFound) {
    //             if (uart_available(nex->uart_port)) {
    //                 _tempChar = uart_read_bytes(nex->uart_port, &_tempChar, 1, portMAX_DELAY);

    //                 if (_tempChar == 0xFF || _tempChar == 0xFFFFFFFF) {
    //                     _endBytes++;
    //                     if (_endBytes == 3) {
    //                         _endOfCommandFound = true;
    //                     }
    //                 } else {
    //                     _numberValue = 777777;
    //                     break;
    //                 }
    //             }

    //             if ((esp_timer_get_time() / 1000) - _tmr1 > 1000UL) {
    //                 _numberValue = 777777;
    //                 break;
    //             }
    //         }
    //     }
    // }

    // if (_endOfCommandFound) {
    //     _numberValue = _numericBuffer[3];
    //     _numberValue <<= 8;
    //     _numberValue |= _numericBuffer[2];
    //     _numberValue <<= 8;
    //     _numberValue |= _numericBuffer[1];
    //     _numberValue <<= 8;
    //     _numberValue |= _numericBuffer[0];
    // } else {
    //     _numberValue = 777777;
    // }

    // return _numberValue;
    return 1;
}

char* EasyNex_readStr(easyNex_t* nex, const char* comp)
{
    // char buffer[128];
    // char *_Textcomp = comp;
    // bool _endOfCommandFound = false;
    // char _tempChar;

    // _tmr1 = esp_timer_get_time() / 1000;

    // while (uart_available(nex->uart_port)) 
    // {
    //     if ((esp_timer_get_time() / 1000) - _tmr1 > 1000UL) 
    //     {
    //         _readString = "ERROR";
    //         break;
    //     } 
    //     else 
    //     {
    //         NextionListen();
    //     }
    // }

    // snprintf(buffer, sizeof(buffer), "get %s\xff\xff\xff", _Textcomp);

    // _tmr1 = esp_timer_get_time() / 1000;
    // while (uart_available(nex->uart_port) < 4) 
    // {
    //     if ((esp_timer_get_time() / 1000) - _tmr1 > 400UL) 
    //     {
    //         _readString = "ERROR";
    //         break;
    //     }
    // }

    // if (uart_available(nex->uart_port) > 3) {
    //     _start_char = uart_read_bytes(nex->uart_port, &_start_char, 1, portMAX_DELAY);
    //     _tmr1 = esp_timer_get_time() / 1000;

    //     while (_start_char != 0x70) {
    //         if (uart_available(nex->uart_port)) {
    //             _start_char = uart_read_bytes(nex->uart_port, &_start_char, 1, portMAX_DELAY);
    //         }

    //         if ((esp_timer_get_time() / 1000) - _tmr1 > 100UL) {
    //             _readString = "ERROR";
    //             break;
    //         }
    //     }

    //     if (_start_char == 0x70) {
    //         _readString = "";
    //         int _endBytes = 0;
    //         _tmr1 = esp_timer_get_time() / 1000;

    //         while (!_endOfCommandFound) {
    //             if (uart_available(nex->uart_port)) {
    //                 _tempChar = uart_read_bytes(nex->uart_port, &_tempChar, 1, portMAX_DELAY);

    //                 if (_tempChar == 0xFF || _tempChar == 0xFFFFFFFF) {
    //                     _endBytes++;
    //                     if (_endBytes == 3) {
    //                         _endOfCommandFound = true;
    //                     }
    //                 } else {
    //                     _readString += _tempChar;
    //                 }
    //             }

    //             if ((esp_timer_get_time() / 1000) - _tmr1 > 1000UL) {
    //                 _readString = "ERROR";
    //                 break;
    //             }
    //         }
    //     }
    // }

    // return _readString.c_str();
    return 1;
}

int EasyNex_readByte(easyNex_t* nex)
{
    // uint8_t data;
    // int len = uart_read_bytes(nex->uart_port, &data, 1, portMAX_DELAY);
    // return (len == 1) ? data : -1;
    return 1;
} 
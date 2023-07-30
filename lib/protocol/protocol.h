/*
 * Copyright (c) 2023, Fabiola de las Casas Escardo <fabioladelascasas@gmail.com>
 * All rights reserved.
 * License: bsd-3-clause (see LICENSE.txt)
 * Version: 1.0
 * 
 */

#ifndef PROTOCOL_PROTOCOL_H_
#define PROTOCOL_PROTOCOL_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
/*******************************************************************************
 * Definitions
******************************************************************************/
#define PROTOCOL_SIZE   7
typedef enum{
    ACTION_BROADCAST,
    ACTION_ONE,
    ACTION_ANSWER 
}action_code_t;

typedef enum{
    FLAG_OK,
    FLAG_ERROR 
}flag_status_t;

typedef struct {
    uint8_t address;
    action_code_t actionCode : 4;
    flag_status_t flagStatus : 4;
    uint32_t payload;
} protocol_frame_t;
typedef enum{
    CRC_OK,
    CRC_ERROR 
}crc_status_t;
/*******************************************************************************
 * API
 ******************************************************************************/
/* 
 * @brief Set variables of frame structure
 * 
 * @param frame
 * @param addres addres of the node
 * @param actionCode 
 * @param flagStatus
 * @param temp temperature measured with sensor
 * @param hum humidity measured with sensor
 * 
 */
void protocol_setFrame(protocol_frame_t *frame, uint8_t address, action_code_t actionCode, flag_status_t flagStatus, 
                        uint16_t temp, uint16_t hum); 

/*
 * @brief Build frame in data buffer to send over communication port
 *
 * @param data_buff buffer to store frame 
 * @param frame structure with information to send
 * @return length of buffer built
 * 
*/                 
uint16_t protocol_buildFrame(char *data_buff, protocol_frame_t *frame);

/*
 * @brief Read buffer received and separate into frame parameters
 *
 * @param data_buff buffer where data was received 
 * @param frame structure to save information received
 * @return status of crc checked value
 * 
*/  
crc_status_t protocol_readFrame(char *data_buff, protocol_frame_t *frame);

#endif /* PROTOCOL_PROTOCOL_H_ */
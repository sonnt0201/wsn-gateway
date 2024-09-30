/*
 * 9/2024 by Thai-Son Nguyen.
 * Queue services for temparature-humidity WSN project.
 */

#ifndef QUEUE_SERVICE_H
#define QUEUE_SERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"


#include "config.h"


// #pragma once
#ifndef Q_ITEMS_NUM
#define Q_ITEMS_NUM 5
#endif

#ifndef Q_ITEM_SIZE
#define Q_ITEM_SIZE 100
#endif

#define Q_OK 0
#define Q_ERROR 1
#define Q_ALREADY_INIT 2
#define Q_EMPTY 3
#define Q_FULL 4
#define Q_NOT_INIT 5

 QueueHandle_t encoded_data_queue = NULL;

// ************* INTERFACE ****************

/*
    Must be called first to init queue

    @return
    - Q_OK if successful
    - Q_ALREADY_INIT if already initialized
*/
uint8_t init_encoded_data_queue();

/*
    * Push data to queue

    * @return
    * - Q_OK if successful
    * - Q_FULL if queue is full
*/
uint8_t push_to_encoded_data_queue(char *buffer);

/*
 * pop data from queue
 *@param
 * buffer: pointer to holding output data
 *
 * @return
 * - Q_OK if successful
 * - Q_EMPTY if queue is empty
 */
uint8_t pop_from_encoded_data_queue(char *buffer);

// ************* IMPLEMENTATION ****************

inline uint8_t init_encoded_data_queue()
{
   
    if (encoded_data_queue == NULL)
    {

        encoded_data_queue = xQueueCreate(Q_ITEMS_NUM, Q_ITEM_SIZE);
        return Q_OK;
    }
    else
    {
        return Q_ALREADY_INIT;
    }
}

inline uint8_t push_to_encoded_data_queue(char *buffer)
{
    if (encoded_data_queue == NULL)
    {
        encoded_data_queue = xQueueCreate(Q_ITEMS_NUM, Q_ITEM_SIZE);
    }

    int rc = xQueueSend(encoded_data_queue, buffer, (TickType_t)0);

    if (rc == pdTRUE)
    {
        return Q_OK;
    }
    else
        return Q_FULL;
}

inline uint8_t pop_from_encoded_data_queue(char *buffer)
{

    if (!encoded_data_queue)
        return Q_NOT_INIT;

    int rc = xQueueReceive(encoded_data_queue, buffer, (TickType_t)1);

    if (rc == pdTRUE)
    {
        return Q_OK;
    }
    else
    {
        return Q_ERROR;
    }
};


#ifdef __cplusplus
}
#endif
#endif // QUEUE_SERVICE_H
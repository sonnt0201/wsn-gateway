#ifndef TASK_LORA_LISTENER
#define TASK_LORA_LISTENER

// ************* INTERFACE ******************
void lora_listener(void *pvParameters);


// ***************** IMPLEMENTATION ***************
#include "esp_system.h"
#include "../lib/encoder.h"
#include "../lib/queue_services.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

void quick_send(
    char* device_id, // 48 bits mac address
    double temperature,
    int humidity
);



inline void quick_send(
    char* device_id, // 48 bits mac address
    double temperature,
    int humidity
){
     char *jsonstr =  encode_th_data(
       device_id,
       temperature,
       humidity
    );

    push_to_encoded_data_queue(jsonstr);
      printf("len: %d \n%s\n", strlen(jsonstr), jsonstr);

};


#endif //TASK_LORA_LISTENER
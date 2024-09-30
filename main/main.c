#include <stdio.h>
#include "../lib/easywifi.h"
#include "../lib/queue_services.h"
#include "../lib/encoder.h"
#include "../lib/easytcp.h"
#include "task_lora_listener.h"
#include "task_th_sender.h"

void app_main(void)
{
    init_encoded_data_queue();
    easywifi_connect();
    xTaskCreatePinnedToCore(lora_listener, "lora_listener", 8196, NULL, 1, NULL, 0); // Chạy task lora_listener ở core 0
    xTaskCreatePinnedToCore(th_sender, "th_sender", 8192, NULL, 13, NULL, 1); // Chạy task th_sender ở core 1 
}

void lora_listener(void *pvParameters){
    
    srand(time(NULL));

    char mac[3][15] = {"12AF347BAABB", "34AF347BAABB", "56AF347BAABB"};

    char *jsonstr = encode_th_data(
        mac[rand() % 3],
        30.1,
        68);

    while (1)
    {
        quick_send(
        mac[rand() % 3],
        (rand() % 90 + 10),
       ( rand() % 98)
        );

        vTaskDelay(pdMS_TO_TICKS(3000));

        // vTaskDelayUntil(&GetTick,10);
      
       
    };
};

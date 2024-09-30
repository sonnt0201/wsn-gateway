#ifndef TH_SENDER_H
#define TH_SENDER_H

#include "../lib/easytcp.h"
#include "../lib/queue_services.h"
void th_sender(void *pvParameters);

inline void th_sender(void *pvParameters) {

      char rxbuff[100];
   
    while (1)
    {
        if (
            pop_from_encoded_data_queue(rxbuff) == Q_OK

        ) {
            send_to_server(rxbuff, strlen(rxbuff));
        };
    };
}


#endif /* TH_SENDER_H */
/*
    * 9/2024 by Thai-Son Nguyen.
    * Tool for easy sending data to server via tcp socket.
*/

#ifndef EASY_TCP_H
#define EASY_TCP_H
#ifdef __cplusplus
extern "C"
{
#endif


#include "config.h"



void send_to_server(char *payload, int len);


// *********** IMPLEMENTATION ************************

#include <string.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

static const char *REQUEST_FORMAT = "POST %s HTTP/1.1\r\n"
    "Host: %s:%s\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: %d\r\n"
    "\r\n"
;



const struct addrinfo hints = {
    .ai_family = AF_INET,
    .ai_socktype = SOCK_STREAM,
};

// hàm ok, đã tự giải phóng các biến nội bộ.
void send_to_server(char *payload, int len)

{

    struct addrinfo *res;
    struct in_addr *addr;
    int s, r;

    char *request = malloc(700);

    int err = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &res);
    if (err != 0 || res == NULL)
    {
        ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
        // vTaskDelay(10 / portTICK_PERIOD_MS);

        //  close(s);
        return;
    }

    addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
    // ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

    s = socket(res->ai_family, res->ai_socktype, 0);
    if (s < 0)
    {
        ESP_LOGE(TAG, "... Failed to allocate socket.");

        close(s);

        // if (payload != NULL)
        // {
        //     free(payload);
        //     payload = NULL;
        // }

        freeaddrinfo(res);
        // vTaskDelay(1 / portTICK_PERIOD_MS);
        return;
    }

    if (connect(s, res->ai_addr, res->ai_addrlen) != 0)
    {
        ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
        //  vTaskDelay(1 / portTICK_PERIOD_MS);
        close(s);

        freeaddrinfo(res);

        if (request != NULL)
        {
            free(request);
            request = NULL;
        }

        return;
    }

    char *body = payload;

    // int request_len = sprintf(request, REQUEST_FORMAT, WEB_PATH, WEB_SERVER, WEB_PORT, len);

    char *request_w_body = body;

    if (write(s, request_w_body, strlen(request_w_body)) < 0)
    {
        ESP_LOGE(TAG, "... socket send failed");

        close(s);

        // try to free all;
        // if (body != NULL)
        // {
        //     free(body);
        //     body = NULL;
        // }
        freeaddrinfo(res);

        if (request != NULL)
        {
            free(request);
            request = NULL;
        }

        vTaskDelay(2 / portTICK_PERIOD_MS);
        return;
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);
    close(s);
    freeaddrinfo(res);

    // if (body != NULL)
    // {
    //     free(body);
    //     body = NULL;
    // }

    if (request != NULL)
    {
        free(request);
        request = NULL;
    }
};


#ifdef __cplusplus
}
#endif
#endif /* WIFI_CONNECT_H */
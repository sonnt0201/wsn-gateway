/*
   * 9/2024 by Thai-Son Nguyen.
   * ESP32 Wifi connecting extension for espidf.
*/

#ifndef EASY_WIFI_H
#define EASY_WIFI_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_log.h"
    // *************** @ DECLARATION - INTERFACES*****************

    // connect to a new wifi
    int easywifi_connect();

    int easywifi_disconnect();

    // *************** DECLARATION - INTERNAL USE *****************

#include "esp_wifi.h"
#include "esp_event.h"

#include "nvs_flash.h"
#include "string.h"


#include "config.h"


    // default wifi configs

#ifndef WIFI_SSID_DEFAULT
#define WIFI_SSID_DEFAULT ""
#endif

#ifndef WIFI_PASSWORD_DEFAULT
#define WIFI_PASSWORD_DEFAULT ""
#endif

    bool _is_wifi_setup = false;
    bool _is_wifi_connected = false;

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID_DEFAULT,
            .password = WIFI_PASSWORD_DEFAULT},
    };

    static char *TAG = "wifi_connect";

    void _wifi_event_group_handler(void *arg, esp_event_base_t event_base,
                                   int32_t event_id, void *event_data);


    void _wifi_setup();

    // *************** IMPLEMENTATIONs *****************

    inline void _wifi_setup()
    {

        if (_is_wifi_setup)
            return;

        nvs_flash_init();
        esp_netif_init();
        esp_event_loop_create_default();

        esp_netif_create_default_wifi_sta();

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        esp_wifi_init(&cfg);

        esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &_wifi_event_group_handler, NULL);
        esp_wifi_set_mode(WIFI_MODE_STA);
        // esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);

        _is_wifi_setup = true;
    };

    inline void _wifi_event_group_handler(void *arg, esp_event_base_t event_base,
                                          int32_t event_id, void *event_data)
    {

        if (event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            printf("after disconnected, ssid: %s, password: %s", wifi_config.sta.ssid, wifi_config.sta.password);
            _is_wifi_connected = false;
            esp_wifi_connect();
            ESP_LOGE(TAG, "Wifi disconnected. Check your wifi and config then reset the device");
        }
    }

    inline int easywifi_connect()
    {

        if (!_is_wifi_setup)
            _wifi_setup();

        if (_is_wifi_connected)
            return 0;

        // //    &(wifi_config.sta.ssid) = strcpy((char *)wifi_config.sta.ssid, ssid);
        // //    &(wifi_config.sta.password) = strcpy((char *)wifi_config.sta.password, password);

     
        //  strcpy((char *)new_wifi_config.sta.ssid, ssid);
        //  strcpy((char *)new_wifi_config.sta.password, password);

        esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
        esp_wifi_start();

        int rc = esp_wifi_connect();

        switch (rc)
        {
        case ESP_OK:

            _is_wifi_connected = true;
            ESP_LOGI(TAG, "Connected to wifi!");

            break;

        case ESP_ERR_WIFI_NOT_INIT:
            ESP_LOGE(TAG, "WiFi is not initialized by esp_wifi_init");
            break;

        case ESP_ERR_WIFI_NOT_STARTED:
            ESP_LOGE(TAG, "WiFi is not started by esp_wifi_start");
            break;

        case ESP_ERR_WIFI_MODE:
            ESP_LOGE(TAG, "WiFi mode error");
            break;

        default:
            ESP_LOGE(TAG, "WiFi error");
            break;
        }

        return rc;
    }

    inline int easywifi_disconnect()
    {
        int rc = esp_wifi_disconnect();

        switch (rc)
        {
        case ESP_OK:

            _is_wifi_connected = false;
            ESP_LOGI(TAG, "Wifi disconnected!");

            break;

        default:
            ESP_LOGE(TAG, "WiFi error, cannot disconnect!");
            break;
        }

        return rc;
    }

#ifdef __cplusplus
}
#endif
#endif /* WIFI_CONNECT_H */
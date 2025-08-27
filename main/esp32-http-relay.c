// #include <string.h>
// #include "esp_log.h"
// #include "esp_http_server.h"
// #include "nvs_flash.h"
// #include "esp_wifi.h"
// #include "esp_event.h"
// #include "esp_netif.h"
// #include "driver/gpio.h"

// #define RELAY_GPIO 2 // Chân relay

// static const char *TAG = "HTTP_RELAY";

// esp_err_t toggle_post_handler(httpd_req_t *req)
// {
//     char content[100];
//     int ret = httpd_req_recv(req, content, sizeof(content) - 1);
//     if (ret <= 0) return ESP_FAIL;
//     content[ret] = 0;

//     ESP_LOGI(TAG, "Received: %s", content);

//     // Nếu chuỗi chứa "true" thì bật, "false" thì tắt
//     if (strstr(content, "true")) {
//         gpio_set_level(RELAY_GPIO, 0);
//     } else {
//         gpio_set_level(RELAY_GPIO, 1);
//     }

//     httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
//     return ESP_OK;
// }

// httpd_uri_t toggle_uri = {
//     .uri       = "/toggle",
//     .method    = HTTP_POST,
//     .handler   = toggle_post_handler,
//     .user_ctx  = NULL
// };

// void start_webserver(void)
// {
//     httpd_config_t config = HTTPD_DEFAULT_CONFIG();
//     httpd_handle_t server = NULL;

//     if (httpd_start(&server, &config) == ESP_OK) {
//         httpd_register_uri_handler(server, &toggle_uri);
//         ESP_LOGI(TAG, "Server started");
//     }
// }

// void wifi_init_sta()
// {
//     esp_netif_init();
//     esp_event_loop_create_default();
//     esp_netif_create_default_wifi_sta();

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     esp_wifi_init(&cfg);
//     wifi_config_t wifi_config = {
//         .sta = {
//             .ssid = "SkyTech",
//             .password = "skytech2024",
//         },
//     };
//     esp_wifi_set_mode(WIFI_MODE_STA);
//     esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
//     esp_wifi_start();
//     esp_wifi_connect();
// }

// void app_main(void)
// {
//     nvs_flash_init();
//     gpio_set_direction(RELAY_GPIO, GPIO_MODE_OUTPUT);
//     gpio_set_level(RELAY_GPIO, 0); // Tắt relay ban đầu

//     wifi_init_sta();
//     start_webserver();
// }
// 

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "esp_tls.h"
#include "cJSON.h"
#include "esp_netif.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"

#define WIFI_SSID      "SkyTech"
#define WIFI_PASS      "skytech2024"
#define TAG            "FIREBASE"
#define FIREBASE_URL FIREBASE_DATABASE_URL "SensorData.json"
#define FIREBASE_DATABASE_URL "https://kiemtramau-6613c-default-rtdb.asia-southeast1.firebasedatabase.app/SensorData.json"



void wifi_init_sta(void) {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    ESP_LOGI(TAG, "Connecting to WiFi...");
    esp_wifi_connect();

    EventBits_t bits;
    while (true) {
        wifi_ap_record_t ap_info;
        if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            ESP_LOGI(TAG, "Connected to SSID: %s", ap_info.ssid);
            break;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void send_to_firebase(float temperature, float humidity) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temperature", temperature);
    cJSON_AddNumberToObject(root, "humidity", humidity);
    cJSON_AddNumberToObject(root, "timestamp", esp_timer_get_time() / 1000000); // seconds

    char *post_data = cJSON_PrintUnformatted(root);

    esp_http_client_config_t config = {
        .url = FIREBASE_URL,
        .method = HTTP_METHOD_PUT,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .cert_pem = NULL, // Tắt xác minh TLS
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Data sent successfully! Status = %d", esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);
    cJSON_Delete(root);
    free(post_data);
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init_sta();

    while (1) {
        float temperature = (rand() % 210) / 10.0 + 15;  // 15 - 35°C
        float humidity = (rand() % 610) / 10.0 + 30;     // 30 - 90%

        ESP_LOGI(TAG, "Sending Temperature: %.2f, Humidity: %.2f", temperature, humidity);
        send_to_firebase(temperature, humidity);

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

/* ESP SMB API

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>

#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

// Network interface
#include "http_server.h"
#include "wifi_app.h"
#include "app_smb.h"
// Storage interface
#include "app_nvs.h"
#include "app_spiffs.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    app_nvs_flash_setup(); 
    assert(app_spiffs_mount_file_system() == LOCAL_DB_SUCCESS); //> Initialize SPIFFS partition

    app_smb_init();

    wifi_app_start();
}

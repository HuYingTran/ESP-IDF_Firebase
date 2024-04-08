#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "iot_var.h"
#include "ctype.h"

#define TAG "iot_wifi"

#define EXAMPLE_ESP_MAXIMUM_RETRY  5

#define TAG "ESP32 WIFI STA"

static void event_handler(void* arg, esp_event_base_t event_base,
								int32_t event_id, void* event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        ESP_LOGI(TAG,"WiFi connecting ...");
        break;
    case WIFI_EVENT_STA_CONNECTED:
		global.wifi_sta.wifi_sta_connected = true;
        ESP_LOGI(TAG,"Connected ssid:%s pass:%s",global.wifi_sta.WIFI_SSID, global.wifi_sta.WIFI_PASSWORD);
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG,"WiFi lost connection");
        break;
    case IP_EVENT_STA_GOT_IP:
        global.wifi_sta.wifi_sta_connected = true;
        ESP_LOGI(TAG,"Got IP ssid:%s pass:%s",global.wifi_sta.WIFI_SSID,global.wifi_sta.WIFI_PASSWORD);
        break;
    default:
        break;
    }
}

void initialise_wifi(void)
{
	esp_log_level_set("wifi", ESP_LOG_WARN);
	static bool initialized = false;
	if (initialized) {
		return;
	}
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
	assert(ap_netif);
	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
	assert(sta_netif);
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &event_handler, NULL) );
	ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );

	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );
	ESP_ERROR_CHECK( esp_wifi_start() );

	initialized = true;
}

void iot_wifi_sta_init()
{
	if(!strcmp(global.wifi_sta.WIFI_SSID,"") || !strcmp(global.wifi_sta.WIFI_SSID,"")) {
		ESP_LOGI(TAG,"SSID PASS NULL");
		return;
	}

    if(global.wifi_sta.wifi_sta_connected){
        global.wifi_sta.wifi_sta_connected = 0;
        esp_wifi_disconnect();
    }
	wifi_config_t wifi_config = { 0 };
	strcpy((char *)wifi_config.sta.ssid, global.wifi_sta.WIFI_SSID);
	strcpy((char *)wifi_config.sta.password, global.wifi_sta.WIFI_PASSWORD);

	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK( esp_wifi_connect());
}
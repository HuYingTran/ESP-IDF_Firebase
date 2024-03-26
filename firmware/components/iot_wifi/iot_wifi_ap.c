#include "esp_log.h"
#include "iot_var.h"

#define TAG "iot_wifi"

#define CONFIG_AP_MAX_STA_CONN 4
#define CONFIG_AP_WIFI_CHANNEL 1

void iot_wifi_ap_init(void)
{
	wifi_config_t wifi_config = { 0 };
	strcpy((char *)wifi_config.ap.ssid,global.wifi_ap.WIFI_SSID);
	strcpy((char *)wifi_config.ap.password, global.wifi_ap.WIFI_PASSWORD);
	wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
	wifi_config.ap.ssid_len = strlen(global.wifi_ap.WIFI_SSID);
	wifi_config.ap.max_connection = CONFIG_AP_MAX_STA_CONN;
	wifi_config.ap.channel = CONFIG_AP_WIFI_CHANNEL;

	if (strlen(global.wifi_ap.WIFI_PASSWORD) == 0) {
		wifi_config.ap.authmode = WIFI_AUTH_OPEN;
	}

	ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_APSTA) );
	ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config) );
	ESP_ERROR_CHECK( esp_wifi_start() );
	ESP_LOGI(TAG, "WIFI_MODE_AP started. SSID:%s password:%s channel:%d",
			 global.wifi_ap.WIFI_SSID, global.wifi_ap.WIFI_PASSWORD, CONFIG_AP_WIFI_CHANNEL);
}
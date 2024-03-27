#include "iot_var.h"
#include "iot_wifi.h"
#include "iot_http.h"
#include "iot_gpio.h"
#include "iot_spiffs.h"
#include "iot_nvs.h"

void iot_wifi_default_inf(){
    strcpy(global.wifi_ap.WIFI_SSID, "Iot Huynh");
    strcpy(global.wifi_ap.WIFI_PASSWORD, "12345678");
    initialise_wifi();
    iot_wifi_ap_init();
    iot_wifi_sta_init();
}

static void iot_load_setting(){
    iot_nvs_read(NVS_KEY_STA_SSID, global.wifi_sta.WIFI_SSID, STR_LENGTH);
    iot_nvs_read(NVS_KEY_STA_PASS, global.wifi_sta.WIFI_PASSWORD, STR_LENGTH);
}

void iot_init()
{
    iot_nvs_init();
    iot_gpio_setting();
    iot_setup_spiffs();

        iot_load_setting();
    global.wifi_change = false;
    global.wifi_sta.wifi_sta_connected = false;
    iot_wifi_default_inf();
    iot_http_server_init();
}
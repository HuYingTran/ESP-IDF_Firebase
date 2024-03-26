#include "iot_var.h"
#include "iot_wifi.h"
#include "iot_http.h"
#include "iot_gpio.h"
#include "iot_spiffs.h"

void iot_wifi_default_inf(){
    global.wifi_ap.WIFI_SSID = "Iot Huynh";
    global.wifi_ap.WIFI_PASSWORD = "12345678";
    initialise_wifi();
    iot_wifi_ap_init();
}

void iot_init()
{
    iot_setup_spiffs();
    gpio_set_level(IOT_GPIO_WIFI,IOT_LOW);
    global.wifi_change = false;
    global.wifi_sta.wifi_sta_connected = false;
    iot_wifi_default_inf();
    iot_http_server_init();
}
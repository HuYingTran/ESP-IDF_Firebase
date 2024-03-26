#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Requires by memset
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "driver/gpio.h"
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>

typedef struct
{
    char *WIFI_SSID;
    char *WIFI_PASSWORD;
    bool wifi_sta_connected;
} wifi_inf;


typedef struct
{
    wifi_inf wifi_sta;
    wifi_inf wifi_ap;
    bool wifi_change;
} global_t;

global_t global;
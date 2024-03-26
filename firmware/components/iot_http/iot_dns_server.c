#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#define TAG "IOT DNS"

void dns_server_task(void *pvParameters)
{
    struct sockaddr_in serverAddr;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        ESP_LOGE(TAG, "Failed to create socket");
        vTaskDelete(NULL);
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(53);

    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        ESP_LOGE(TAG, "Failed to bind socket");
        close(sock);
        vTaskDelete(NULL);
        return;
    }

    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        char dns_buffer[1024];
        int len = recvfrom(sock, dns_buffer, sizeof(dns_buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (len > 0) {
            dns_respserver(sock, dns_buffer, len);
        }
    }
}

void dns_respserver(int sock, char *dns_buffer, int len)
{
    struct sockaddr_in destAddr;
    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = ((struct sockaddr_in *)&destAddr)->sin_port;
    inet_pton(AF_INET, "192.168.4.1", &destAddr.sin_addr);

    sendto(sock, dns_buffer, len, 0, (struct sockaddr *)&destAddr, sizeof(destAddr));
}

/* Start DNS server task */
void start_dns_server_task()
{
    xTaskCreate(&dns_server_task, "dns_server_task", 4096, NULL, 5, NULL);
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id) {
        case SYSTEM_EVENT_AP_START:
            ESP_LOGI(TAG, "Access point started");
            start_dns_server_task();
            break;
        default:
            break;
    }
    return ESP_OK;
}
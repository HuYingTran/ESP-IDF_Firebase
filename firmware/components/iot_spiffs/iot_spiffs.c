#include "stdio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <esp_http_server.h>
#include "iot_spiffs.h"

#define TAG "IOT SPIFFS"

esp_err_t iot_setup_spiffs()
{
    // Khởi tạo và định dạng SPIFFS
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "explorer",
        .max_files = 5,
        .format_if_mount_failed = false};

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            printf("Failed to mount or format filesystem\n");
        }
        else if (ret == ESP_ERR_NOT_FOUND)
        {
            printf("Failed to find SPIFFS partition\n");
        }
        else
        {
            printf("Failed to initialize SPIFFS (%s)\n", esp_err_to_name(ret));
        }
    }
    printf("\ninitialize SPIFFS\n");

    ESP_LOGI(TAG, "Performing SPIFFS_check().");
    ret = esp_spiffs_check(conf.partition_label);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "SPIFFS_check() successful");
    }
    return ret;
}

void iot_spiffs_readfile(const char* file, char *buf){
    char path_file[32] = "/spiffs/";
    strcat(path_file,file);

    FILE *f = fopen(path_file, "r");
    if (f == NULL) {
        printf("Failed to open file for reading\n");
        return;
    }
    // Tạo một vùng nhớ đệm để lưu trữ dữ liệu từ tệp
    size_t bytes_read;
    // Đọc dữ liệu từ tệp và lưu vào vùng nhớ đệm
    bytes_read = fread(buf, 1, BUFFER_SIZE, f);
    fclose(f);
    buf[bytes_read] = '\0';
}
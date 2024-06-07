#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"
#include "string.h"
#include "iot_nvs.h"
#include "nvs.h"

#define TAG "IOT NVS"

void iot_nvs_init(){
    if(nvs_flash_secure_init(NULL) == ESP_OK)
    {
        ESP_LOGI(TAG,"iot_NVS inited");
    }
}

esp_err_t iot_nvs_write(const char *key, const char *value){
    nvs_handle my_handle;
    esp_err_t err = nvs_open(NVS_IOT_NAMESPACE, NVS_READWRITE, &my_handle);
    if(err != ESP_OK)   return err;
    nvs_set_str(my_handle, key, value);
    err = nvs_commit(my_handle);
    nvs_close(my_handle);
    return err;
}

esp_err_t iot_nvs_read(const char *key, char *buf, size_t length){
    nvs_handle my_handle;
    esp_err_t err = nvs_open(NVS_IOT_NAMESPACE, NVS_READONLY, &my_handle);
    if(err != ESP_OK)   return err;
    char *tmp = (char *)malloc(length);
    err = nvs_get_str(my_handle, key, tmp, &length);
    if(err == ESP_OK) {
        for(int i=0; i<length; i++){
            buf[i] = tmp [i];
        }
    }
    free(tmp);
    nvs_close(my_handle);
    return err;
}
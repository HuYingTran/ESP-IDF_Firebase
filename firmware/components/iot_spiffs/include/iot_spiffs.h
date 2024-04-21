#include "esp_err.h"
#define BUFFER_SIZE 1536

esp_err_t iot_setup_spiffs();
void iot_spiffs_readfile(const char* file, char *buf);
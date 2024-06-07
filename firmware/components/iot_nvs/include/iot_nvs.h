#define NVS_IOT_NAMESPACE "IOT_NVS"

#define NVS_KEY_STA_SSID "sta_ssid"
#define NVS_KEY_STA_PASS "sta_pass"

void iot_nvs_init();
esp_err_t iot_nvs_write(const char *key, const char *value);
esp_err_t iot_nvs_read(const char *key, char *buf, size_t length);
void read_file_html();
#include <esp_http_server.h>
#include <esp_log.h>

httpd_handle_t iot_http_server_init();
void iot_stop_server(httpd_handle_t server);

void iot_http_client_get(char *domain, char *path, char *query_params);
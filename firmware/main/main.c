#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "iot_controller.h"
#include "iot_controller_wifi.h"
#include "iot_wifi.h"
#include "iot_http.h"
#include "iot_gpio.h"

void app_main()
{
	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK( nvs_flash_erase() );
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);
	// iot_gpio_pwm(4000);
    iot_init();

    iot_loop();
	printf("\nend");
}

// HTTP Client - FreeRTOS ESP IDF - GET

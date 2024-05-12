#include "driver/gpio.h"
#include "iot_gpio.h"
#include "iot_var.h"
#include "driver/ledc.h"

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_HS_CH1_CHANNEL    LEDC_CHANNEL_1
#define LEDC_TEST_DUTY         (4000)

#define GPIO_PWM GPIO_NUM_13

static int duty_cycle = 0;

void gpio_pwm_init()
{
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           // timer mode
        .timer_num = LEDC_HS_TIMER,           // timer index
        .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel    = LEDC_HS_CH0_CHANNEL,
        .duty       = 0,
        .gpio_num   = GPIO_NUM_13,
        .speed_mode = LEDC_HS_MODE,
        .timer_sel  = LEDC_HS_TIMER
    };
    ledc_channel_config(&ledc_channel);

    while (1) {
        if(global.relay_status[0] == 0){
           ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0); 
        }else{
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty_cycle);
        }
        
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
    }
}

void iot_pwm(){
    xTaskCreate(gpio_pwm_init, "gpio_pwm_init", 2048, NULL, 5, NULL);
}

void iot_gpio_pwm(int d){
    duty_cycle = d;
}

void iot_gpio_setting()
{
    gpio_pad_select_gpio(IOT_GPIO_WIFI);
    gpio_set_direction(IOT_GPIO_WIFI, GPIO_MODE_OUTPUT);
    gpio_set_level(IOT_GPIO_WIFI,IOT_LOW);
    // gpio_set_pull_mode(IOT_GPIO_WIFI, GPIO_PULLDOWN_ONLY);

    // Setup gpio relay mode output
    for(int i = 0; i<4; i++)
    {
        gpio_pad_select_gpio(global.IOT_GPIO_RELAY[i]);
        gpio_set_direction(global.IOT_GPIO_RELAY[i], GPIO_MODE_OUTPUT);
        gpio_set_level(global.IOT_GPIO_RELAY[i],IOT_HIGH);
    }
}
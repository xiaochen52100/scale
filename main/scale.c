#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "modbusrtu.h"
#include "HX711.h"
#include "gatts.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "nvs_app.h"
#include "station_app.h"
#include "tcp_server.h"
#include "uart_app.h"

_mbdata_st mbdata;
Parameter parameter = {
    .ssid = "K2P_2_4G",
    .password = "378540108",
    .ble_name = "ESP_BLE",
    .baud = 4,
    .mode = 5,
    .station = 1,
    .coefficient = 103,
    .zero_error = 0,
    .skin = 0,

};
static void set_mbdata()
{
    //set_config_param();
    memset(&mbdata, 0, sizeof(mbdata));
    get_config_param();
    memcpy(mbdata.buf, &parameter, sizeof(parameter)); //将掉电不丢失的数据拷贝进数组
}

void app_main(void)
{
    //init nvs flash
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    set_mbdata();
    //work mode config
    if ((parameter.mode & 0x01) == 0x01) //232
    {
        ESP_LOGI("MODE CONFIG:", "RS232");
        uart1_init(parameter.baud);
        xTaskCreate(uart1_rx_task, "uart1_rx_task", 1024 * 4, NULL, configMAX_PRIORITIES, NULL);
    }
    if ((parameter.mode & 0x02) == 0x02) //485
    {
        ESP_LOGI("MODE CONFIG:", "RS485");
        uart2_init(parameter.baud);
        xTaskCreate(uart2_rx_task, "uart2_rx_task", 1024 * 4, NULL, configMAX_PRIORITIES, NULL);
    }
    if ((parameter.mode & 0x04) == 0x04) //wifi
    {
        ESP_LOGI("MODE CONFIG:", "WIFI");
        wifi_init(parameter.ssid, parameter.password);
        xTaskCreate(tcp_server_task, "tcp_server", 1024 * 6, (void *)AF_INET, 5, NULL);
    }
    if ((parameter.mode & 0x08) == 0x08) //BLE
    {
        ESP_LOGI("MODE CONFIG:", "BLE");
        gatts_app();
    }
    xTaskCreate(get_weight_task, "get_weight_task", 1024*2, (void *)AF_INET, 5, NULL);

    // while (1)
    // {
    //     vTaskDelay(300 / portTICK_PERIOD_MS); //延时1s
    // }
}


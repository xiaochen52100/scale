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
#include "nvs_app.h"
#include "uart_app.h"
static const int RX_BUF_SIZE = 256;

void uart1_init(uint16_t baud)
{
    uart_config_t uart_config = {
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    if (baud==1)
    {
        uart_config.baud_rate=9600;
    }
    else if (baud==2)
    {
        uart_config.baud_rate=19200;
    }
    else if (baud==3)
    {
        uart_config.baud_rate=38400;
    }
    else if (baud==4)
    {
        uart_config.baud_rate=115200;
    }
    
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD1_PIN, RXD1_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}
int uart1SendData(const _serialbuf_st senddata)
{
    char *buf= (char *)malloc(senddata.len+1);
    memcpy(buf,senddata.buf,senddata.len);
    const int txBytes = uart_write_bytes(UART_NUM_1, buf, senddata.len);
    return txBytes;
}
void uart1_rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    //uint8_t senddata[512];// = (uint8_t *)malloc(RX_BUF_SIZE + 1);
    _serialbuf_st serialbuf;
    _serialbuf_st senddata;
    while (1)
    {
        memset(&senddata,0,sizeof(_serialbuf_st));
        serialbuf.len = uart_read_bytes(UART_NUM_1, serialbuf.buf, RX_BUF_SIZE, 10 / portTICK_RATE_MS);
        if (serialbuf.len>0)
        {
            //ESP_LOGI("serial  ", "len %d", serialbuf.len);
            esp_log_buffer_hex("serial  ", serialbuf.buf, serialbuf.len);
            smb_recvHoldingReg(&mbdata,serialbuf,&senddata);
            //ESP_LOGI("serial  ", "len %d", senddata.len);
            //esp_log_buffer_hex("serial  ", senddata.buf, senddata.len);
            uart1SendData(senddata);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

    }
}
void uart2_init(uint16_t baud)
{
    uart_config_t uart_config = {
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_APB,
    };
    if (baud==1)
    {
        uart_config.baud_rate=9600;
    }
    else if (baud==2)
    {
        uart_config.baud_rate=19200;
    }
    else if (baud==3)
    {
        uart_config.baud_rate=38400;
    }
    else if (baud==4)
    {
        uart_config.baud_rate=115200;
    }

    // Install UART driver (we don't need an event queue here)
    // In this example we don't even use a buffer for sending data.
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, RX_BUF_SIZE * 2, 0, 0, NULL, 0));

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));

    // Set UART pins as per KConfig settings
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, TXD2_PIN, RXD2_PIN, MODRXorTX_GPIO, UART_PIN_NO_CHANGE));

    // Set RS485 half duplex mode
    ESP_ERROR_CHECK(uart_set_mode(UART_NUM_2, UART_MODE_RS485_HALF_DUPLEX));
    
    // Set read timeout of UART TOUT feature
    ESP_ERROR_CHECK(uart_set_rx_timeout(UART_NUM_2, 3));

}
int uart2SendData(const _serialbuf_st senddata)
{
    char *buf= (char *)malloc(senddata.len+1);
    memcpy(buf,senddata.buf,senddata.len);
    const int txBytes = uart_write_bytes(UART_NUM_2, buf, senddata.len);
    return txBytes;
}
void uart2_rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    _serialbuf_st serialbuf;
    _serialbuf_st senddata;
    while (1)
    {
        memset(&senddata,0,sizeof(_serialbuf_st));
        serialbuf.len = uart_read_bytes(UART_NUM_2, serialbuf.buf, RX_BUF_SIZE, 100 / portTICK_RATE_MS);
        if (serialbuf.len>0)
        {
            //ESP_LOGI("serial  ", "len %d", serialbuf.len);
            esp_log_buffer_hex("serial  rcv buf", serialbuf.buf, serialbuf.len);
            smb_recvHoldingReg(&mbdata,serialbuf,&senddata);
            //ESP_LOGI("serial  ", "len %d", senddata.len);
            esp_log_buffer_hex("serial  send buf", senddata.buf, senddata.len);
            uart2SendData(senddata);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

    }
}
/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
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
#include "modbusrtu.h"
#include "HX711.h"
#include "gatts.h"

#define TXD_PIN (GPIO_NUM_22)
#define RXD_PIN (GPIO_NUM_23)

static const int RX_BUF_SIZE = 512;
static _mbdata_st mbdata;

static void set_mbdata()
{
    mbdata.buf[0]=0x0000;
    mbdata.buf[1]=0x0001;
    mbdata.buf[2]=0x0002;
    mbdata.buf[3]=0x0003;
    mbdata.buf[4]=0x0004;
    mbdata.buf[5]=0x0005;
}
static void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}
static int serialSendData(const _serialbuf_st senddata)
{
    char *buf= (char *)malloc(senddata.len+1);
    memcpy(buf,senddata.buf,senddata.len);
    const int txBytes = uart_write_bytes(UART_NUM_1, buf, senddata.len);
    return txBytes;
}
static void rx_task(void *arg)
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
            //smb_recvHoldingReg(&mbdata,serialbuf,&senddata);
            //ESP_LOGI("serial  ", "len %d", senddata.len);
            //esp_log_buffer_hex("serial  ", senddata.buf, senddata.len);
            //serialSendData(senddata);
            //vTaskDelay(50 / portTICK_PERIOD_MS);
        }

    }
}

#define M 10
unsigned int filter2( void )
{
    unsigned int value_buf[M];
    unsigned int count, i, j, temp;
    for( count = 0; count < M; count++ )
    {
        value_buf[count] = Get_Weight();
        vTaskDelay(110 / portTICK_PERIOD_MS);
    }
    for( j = 0; j < M - 1; j++ )
    {
        for( i = 0; i < M - j - 1; i++ )
        {
            if( value_buf[i] > value_buf[i + 1] )
            {
                temp = value_buf[i];
                value_buf[i] = value_buf[i + 1];
                value_buf[i + 1] = temp;
            }
        }
    }
    return value_buf[( M - 1 ) / 2];
}

void app_main(void)
{
    long gross=0;//毛重
    long Weight = 0;
    printf("system start!\n");
    set_mbdata();
    uart_init();
    xTaskCreate(rx_task, "uart_rx_task", 1024*4, NULL, configMAX_PRIORITIES, NULL);
    gatts_app();
    // Init_Hx711();
    // gross = Get_Weight();	//计算放在传感器上的重物重量
    char *data="12345";
    //uint8_t readbuf[5]={0,0,0,0,0};
    while (1)
    {
	    // printf("%d",filter2());	//串口显示重量
	    // printf(" g\n");	//显示单位
	    // printf("\n");		//显示单位
        
        uart_write_bytes(UART_NUM_1, data, 5);
        // vTaskDelay(50 / portTICK_PERIOD_MS);				//延时1s
        // int len=uart_read_bytes(UART_NUM_1, readbuf, 5, 10 / portTICK_RATE_MS);
        // if (len>0)
        // {
        //     esp_log_buffer_hex("serial  ", readbuf, 5);
        // }
        
	    vTaskDelay(1300 / portTICK_PERIOD_MS);				//延时1s

        
    }
    
}

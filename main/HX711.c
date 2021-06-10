#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "driver/gpio.h"
#include "HX711.h"

int32_t weight_real = 0;
unsigned long adc = 0;
unsigned long adc_old = 0;
unsigned long hx711_read(void);

//****************************************************
//初始化HX711
//****************************************************
void init_hx711()
{
	gpio_pad_select_gpio(HX711_SCK);
	gpio_set_direction(HX711_SCK, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(HX711_DT);
	gpio_set_direction(HX711_DT, GPIO_MODE_INPUT);
}

//****************************************************
//称重
//****************************************************
unsigned long get_weight()
{
	adc = hx711_read();
	//weight_real = (long)((float)weight_real/GapValue);
	return adc;
}

//****************************************************
//读取HX711
//****************************************************
unsigned long hx711_read(void) //增益128
{
	unsigned long count;
	unsigned char i;

	gpio_set_level(HX711_DT, 1);
	vTaskDelay(1 / portTICK_PERIOD_MS);

	gpio_set_level(HX711_SCK, 0);
	vTaskDelay(1 / portTICK_PERIOD_MS);

	count = 0;
	while (gpio_get_level(HX711_DT))
		;
	for (i = 0; i < 24; i++)
	{
		gpio_set_level(HX711_SCK, 1);
		vTaskDelay(1 / portTICK_PERIOD_MS);
		count = count << 1;
		gpio_set_level(HX711_SCK, 0);
		vTaskDelay(1 / portTICK_PERIOD_MS);
		if (gpio_get_level(HX711_DT))
			count++;
	}
	gpio_set_level(HX711_SCK, 1);
	count ^= 0x800000;
	vTaskDelay(1 / portTICK_PERIOD_MS);
	gpio_set_level(HX711_SCK, 0);
	vTaskDelay(1 / portTICK_PERIOD_MS);

	return (count);
}

unsigned long filter_weight(void)
{
	unsigned long value_buf[M];
	int count, i, j, temp;
	for (count = 0; count < M; count++)
	{
		value_buf[count] = get_weight();
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
	for (j = 0; j < M - 1; j++)
	{
		for (i = 0; i < M - j - 1; i++)
		{
			if (value_buf[i] > value_buf[i + 1])
			{
				temp = value_buf[i];
				value_buf[i] = value_buf[i + 1];
				value_buf[i + 1] = temp;
			}
		}
	}
	return value_buf[(M - 1) / 2];
}
void get_weight_task(void *arg)
{
	int count = 0;
	static const char *get_weight_TAG = "get_weight_TAG";
	esp_log_level_set(get_weight_TAG, ESP_LOG_INFO);
	init_hx711();
	while (1)
	{
		adc = filter_weight();
		weight_real = (((float)adc / parameter.coefficient * 10) - parameter.zero_error - parameter.skin);
		mbdata.buf[64] = weight_real & 0xffff;
		mbdata.buf[65] = (weight_real >> 16) & 0xffff; //低半字在前
		ESP_LOGI(get_weight_TAG, "WEIGHT: %f g adc:%ld\n", weight_real / 10.0, adc);
		if (count > 20)
		{
			if ((weight_real > 10) && (weight_real < 50)) //空载时自动修正零误差
			{
				parameter.zero_error = ((float)adc / parameter.coefficient) * 10;
				set_config_param(); //保存数据
			}
			if ((weight_real > -50) && (weight_real < -10)) //空载时自动修正零误差
			{
				parameter.zero_error = ((float)adc / parameter.coefficient) * 10;
				set_config_param(); //保存数据
			}
			count = 0;
		}
		count++;
		vTaskDelay(200 / portTICK_PERIOD_MS); //delay 200MS
	}
}
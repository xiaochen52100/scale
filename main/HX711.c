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
#include "HX711.h"

long HX711_Buffer = 0;
long Weight_Maopi = 0,Weight_Shiwu = 0;

#define GapValue 430

//****************************************************
//初始化HX711
//****************************************************
void Init_Hx711()
{
	gpio_pad_select_gpio(HX711_SCK);
    gpio_set_direction(HX711_SCK, GPIO_MODE_OUTPUT);
	gpio_pad_select_gpio(HX711_DT);
    gpio_set_direction(HX711_DT, GPIO_MODE_INPUT);
}


//****************************************************
//获取毛皮重量
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();		
} 

//****************************************************
//称重
//****************************************************
long Get_Weight()
{
	HX711_Buffer = HX711_Read();
	Weight_Shiwu = HX711_Buffer;
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;				//获取实物的AD采样数值。
	//Weight_Shiwu = (long)((float)Weight_Shiwu/GapValue); 	
	return Weight_Shiwu;
}

//****************************************************
//读取HX711
//****************************************************
unsigned long HX711_Read(void)	//增益128
{
	unsigned long count; 
	unsigned char i;

	gpio_set_level(HX711_DT, 1);
	vTaskDelay(1 / portTICK_PERIOD_MS);

	gpio_set_level(HX711_SCK, 0);
	vTaskDelay(1 / portTICK_PERIOD_MS);

  	count=0; 
  	while(gpio_get_level(HX711_DT)); 
  	for(i=0;i<24;i++)
	{ 
	  	gpio_set_level(HX711_SCK, 1); 
		vTaskDelay(1 / portTICK_PERIOD_MS);
	  	count=count<<1; 
		gpio_set_level(HX711_SCK, 0); 
		vTaskDelay(1 / portTICK_PERIOD_MS);
	  	if(gpio_get_level(HX711_DT))
			count++; 
	} 
 	gpio_set_level(HX711_SCK, 1); 
	count ^= 0x800000;
	vTaskDelay(1 / portTICK_PERIOD_MS);
	gpio_set_level(HX711_SCK, 0); 
	vTaskDelay(1 / portTICK_PERIOD_MS);
	
	return(count);
}

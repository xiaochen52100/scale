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
#include "nvs.h"
#include "nvs_flash.h"
#include "nvs_app.h"


void get_config_param(void)
{  
    esp_err_t err;
    nvs_handle_t config_handle;   
    size_t len;
 
    err = nvs_open("deviceParameter", NVS_READWRITE, &config_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        printf("opening NVS handle Done\n");
        // Read
        printf("Get parameter and key from NVS ... \n");
        //get parameter
        len = sizeof(parameter);
        err = nvs_get_blob(config_handle, "parameter", &parameter, &len);
        if(err==ESP_OK)
        {
            printf("Get parameter success!\n");
            printf("parameter_len=%d\n",len);
        }
        else
        {
           printf("get err =0x%x\n",err);
           printf("Get parameter fail!\n");
        }
        err = nvs_commit(config_handle);
        printf((err != ESP_OK) ? "nvs_commit Failed!\n" : " nvs_commit Done\n");
    }
    
    // Close
    nvs_close(config_handle);
}
 
void set_config_param(void)
{
    esp_err_t err;
    nvs_handle_t config_handle;  
    
    err = nvs_open("deviceParameter", NVS_READWRITE, &config_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 
    else 
    {
        printf("opening NVS handle Done\n");
        printf("Set parameter from NVS ... \n");
        //write parameter
        err=nvs_set_blob(config_handle,"parameter",&parameter,sizeof(parameter));
        if(err==ESP_OK)
            printf("set parameter success!\n");
        else
        {
            printf("set parameter fail!\n");
        }   
        err = nvs_commit(config_handle);
        printf((err != ESP_OK) ? "nvs_commit Failed!\n" : "nvs_commit Done\n");
    }
     // Close
    nvs_close(config_handle);
    printf("--------------------------------------\n");
}

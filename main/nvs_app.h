#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "nvs_app.h"
#include "modbusrtu.h"
#include <stdint.h>
typedef struct _parameter   //掉电不丢失参数，//对应mbdata0-64
{
    char ssid[32];      //wifi名称
    char password[32];  //wifi密码
    char ble_name[32];      //wifi名称
    uint16_t baud;      //波特率    1:9600  2:19200  3:38400 4:115200
    uint16_t mode;      //工作模式 低四位  0：232  1：485 2：wifi 3:BLE 
    uint16_t station;   //站号   
    uint16_t coefficient;   //adc转重量系数
    int32_t zero_error;  //零误差
    uint32_t skin;        //皮重

}Parameter;
typedef struct _realtime_value  //对应mbdata63-80
{
    int32_t weight;      //重量 单位g
    uint16_t coefficient; //置零 执行完后会自动复位
    uint16_t remove_skin; //去皮 执行完后会自动复位  
    uint16_t calibrate_start; //开始校准 当被置1后 程序会记录当前adc值 
    uint16_t calibrate_weight;//校准砝码重量，单位g 当此接收到校准重量后 程序会计算重量转换系数 之后将开始校准位复位
}Realtime_value;
extern Parameter parameter;
extern _mbdata_st mbdata;
void get_config_param(void);
void set_config_param(void);
#ifdef __cplusplus
}
#endif
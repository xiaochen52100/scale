#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <string.h>
typedef struct _parameter
{
    char ssid[32];      //wifi名称
    char password[32];  //wifi密码
    char ble_name[32];      //wifi名称
    uint16_t bund;      //波特率
    uint16_t mode;      //工作模式 低三位，0关闭，1开启  0：串口  1：wifi 2：蓝牙 
    uint16_t station;   //站号   
    uint16_t coefficient;   //adc转重量系数
    uint16_t zero_error;  //零误差
    uint16_t skin;        //皮重

}Parameter;
typedef struct _realtime_value
{
    uint16_t weight;      //重量 单位g
    uint16_t coefficient; //置零 执行完后会自动复位
    uint16_t remove_skin; //去皮 执行完后会自动复位  

}Realtime_value;
void GetConfigParam(void);
void SetConfigParam(void);
#ifdef __cplusplus
}
#endif
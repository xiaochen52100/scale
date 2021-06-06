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
    uint16_t station;   //站号   
    uint16_t bund;      //波特率 1：9600 2：
    uint16_t mode;      //工作模式 低三位，0关闭，1开启  0：串口  1：wifi 2：蓝牙 

}parameter;


#ifdef __cplusplus
}
#endif
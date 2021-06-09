#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "nvs_app.h"
#include <stdint.h>
#define HX711_SCK 16
#define HX711_DT 17
#define M 10

#define RT_WEIGHT mbdata.buf[64]

void get_weight_task(void *arg);
extern unsigned long adc;
extern unsigned long adc_old;
#ifdef __cplusplus
}
#endif
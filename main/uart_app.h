#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "nvs_app.h"
#define TXD1_PIN (GPIO_NUM_23)
#define RXD1_PIN (GPIO_NUM_22)
#define TXD2_PIN (GPIO_NUM_19)
#define RXD2_PIN (GPIO_NUM_18)
void uart1_init(uint16_t baud);
void uart1_rx_task(void *arg);
void uart2_init(uint16_t baud);
void uart2_rx_task(void *arg);
#ifdef __cplusplus
}
#endif
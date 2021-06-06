#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define HX711_SCK 16
#define HX711_DT 17

extern void Init_Hx711();
extern unsigned long HX711_Read(void);
extern long Get_Weight();
extern void Get_Maopi();

#ifdef __cplusplus
}
#endif
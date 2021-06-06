#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define res_ERR1 0x01
#define res_ERR2 0x02
#define res_ERR3 0x03
#define res_ERR4 0x04
#define res_ERR5 0x05
#define res_OK 0x00
#define MODRXorTX_GPIO 2
typedef struct mbdata_st
{
    uint16_t start;
    uint16_t len;
    uint16_t buf[128];
}_mbdata_st;
typedef struct mbfrm_st
{
    uint16_t addr;
    uint16_t cmd;
    uint16_t rlen;
    uint16_t crc;
    uint16_t start;
}_mbfrm_st;
typedef struct serialbuf_st
{
    uint16_t len;
    uint8_t buf[512];
}_serialbuf_st;
uint8_t smb_recvHoldingReg(_mbdata_st *pmb,_serialbuf_st serialRXbuf_st,_serialbuf_st *tx);
#ifdef __cplusplus
}
#endif
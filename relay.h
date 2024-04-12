#ifndef __RELAY_H__
#define __RELAY_H__
#include <Arduino.h>
#include  "main.h"

#define RELAYS_PER_GROUP  4
typedef enum 
{
    VA_RELAY_UNDEF = 0,
    VA_RELAY_TK,
    VA_RELAY_ET,
    VA_RELAY_WC_1,
    VA_RELAY_WC_2,
    VA_RELAY_TUPA_1,
    VA_RELAY_TUPA_2,
    VA_RELAY_MH1_1,
    VA_RELAY_MH1_2,
    VA_RELAY_MH1_3,
    VA_RELAY_MH1_4,
    VA_RELAY_MH1_5,
    VA_RELAY_MH2_1,
    VA_RELAY_MH2_2,
    VA_RELAY_KEITTIO_1,
    VA_RELAY_KEITTIO_2,
    VA_RELAY_KEITTIO_3,
    VA_RELAY_KEITTIO_4,
    VA_RELAY_KEITTIO_5,
    VA_RELAY_KHH_1,
    VA_RELAY_KHH_2,
    VA_RELAY_PSH,
    VA_RELAY_SAUNA,
    VA_RELAY_PARVI,
    VA_RELAY_PIHA,
    VA_RELAY_TERASSI,
    VA_RELAY_POLKU,
    VA_RELAY_NBR_OF
} va_relays_et;

typedef enum
{
    RELAY_GR_BIT_ALL      = 0x0001UL,
    RELAY_GR_BIT_ARRIVE   = 0x0002UL,
    RELAY_GR_BIT_TUPA     = 0x0004UL,
    RELAY_GR_BIT_MH1      = 0x0008UL,
    RELAY_GR_BIT_MH2      = 0x0010UL,
    RELAY_GR_BIT_K        = 0x0020UL,
    RELAY_GR_BIT_KHH      = 0x0040UL,
    RELAY_GR_BIT_ULKO     = 0x0080UL,
    RELAY_GR_BIT_WC       = 0x00100UL,
    //RELAY_GR_BIT_   = 0x0000,
} relay_group_nbit_et;

typedef enum
{
    VA_RELAY_GROUP_ALL  = 0,
    VA_RELAY_GROUP_ARRIVE,
    VA_RELAY_GROUP_TUPA,
    VA_RELAY_GROUP_MH1,
    VA_RELAY_GROUP_MH2,
    VA_RELAY_GROUP_K,
    VA_RELAY_GROUP_KHH,
    VA_RELAY_GROUP_ULKO,
    VA_RELAY_GROUP_WC,
    VA_RELAY_GROUP_NBR_OF   
} va_relay_group_et;

typedef enum
{
    RELAY_CMND_ON       = '1',
    RELAY_CMND_OFF      = '0',
    RELAY_CMND_TOGGLE   = 'T' 
} uart_relay_cmnd_et;


typedef struct
{
    char    unit[4];
    char    relay[6];
    uint8_t state;
    uint16_t  group_map;
} relay_addr_st;


const relay_addr_st *relay_get_addr_ptr( va_relays_et relay_id);

const va_relays_et relay_get_group_item(va_relay_group_et gindx, uint8_t rindx);

void relay_send_one(va_relays_et rindx, char value);

bool relay_get_is_relay_in_group(va_relays_et rindx, uint8_t gindx );


#endif
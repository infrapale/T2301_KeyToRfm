
  
#include "relay.h"
#include "main.h"

const int16_t relay_groups[VA_RELAY_GROUP_NBR_OF] =
{
    [VA_RELAY_GROUP_ALL]      = RELAY_GR_BIT_ALL,
    [VA_RELAY_GROUP_ARRIVE]   = RELAY_GR_BIT_ARRIVE,
    [VA_RELAY_GROUP_TUPA]     = RELAY_GR_BIT_TUPA,
    [VA_RELAY_GROUP_MH1]      = RELAY_GR_BIT_MH1,
    [VA_RELAY_GROUP_MH2]      = RELAY_GR_BIT_MH2,
    [VA_RELAY_GROUP_K]        = RELAY_GR_BIT_K,
    [VA_RELAY_GROUP_KHH]      = RELAY_GR_BIT_KHH,
    [VA_RELAY_GROUP_ULKO]     = RELAY_GR_BIT_ULKO,
};


relay_addr_st relay_addr[VA_RELAY_NBR_OF] =
{
    [VA_RELAY_UNDEF] =      {"XXX", "XXXXX", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_TK] =         {"TK1", "RTK__", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_ET] =         {"TK1", "RET_1", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_WC_1] =       {"TK1", "RWC_1", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_WC_2] =       {"MH2", "RWC_2", 0, RELAY_GR_BIT_ALL},
    [VA_RELAY_TUPA_1] =     {"TK1", "RTUP1", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_TUPA_2] =     {"TK1", "RTUP2", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_MH1_1] =      {"MH1", "RMH11", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_MH1_2] =      {"MH1", "RMH12", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_MH1},
    [VA_RELAY_MH1_3] =      {"MH1", "RMH13", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_MH1},
    [VA_RELAY_MH1_4] =      {"MH1", "RMH14", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_MH1},
    [VA_RELAY_MH1_5] =      {"MH1", "RMH15", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_MH2_1] =      {"MH2", "RMH21", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_MH2},
    [VA_RELAY_MH2_2] =      {"MH2", "RMH22", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_MH2},
    [VA_RELAY_KEITTIO_1] =  {"MH1", "RKOK1", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_K},
    [VA_RELAY_KEITTIO_2] =  {"MH2", "RKOK2", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_K},
    [VA_RELAY_KEITTIO_3] =  {"MH2", "RKOK3", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_K},
    [VA_RELAY_KEITTIO_4] =  {"MH2", "RKOK4", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_K},
    [VA_RELAY_KEITTIO_5] =  {"MH2", "RKOK5", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE},
    [VA_RELAY_KHH_1] =      {"TK1", "RKHH1", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_KHH},
    [VA_RELAY_KHH_2] =      {"MH2", "RKHH2", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_KHH},
    [VA_RELAY_PSH] =        {"MH2", "RPSH_", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_KHH},
    [VA_RELAY_SAUNA] =      {"TK1", "RSAUN", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_KHH},
    [VA_RELAY_PARVI] =      {"TK1", "RPARV", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE },
    [VA_RELAY_PIHA] =       {"TK1", "RPIHA", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_ULKO},
    [VA_RELAY_TERASSI] =    {"TK1", "RTERA", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_ULKO},
    [VA_RELAY_POLKU] =      {"TK1", "RPOLK", 0, RELAY_GR_BIT_ALL | RELAY_GR_BIT_ARRIVE | RELAY_GR_BIT_ULKO},
};


void relay_send_one(va_relays_et rindx, char value)
{
    // <#X1N:RMH1;RKOK1;T;->\n  
    // {"Z":"MH1","S":"RKOK1","V":"T","R":"-"}
    Serial.printf("%s %s %c\n", relay_addr[rindx].unit,relay_addr[rindx].relay, value);
    Serial.printf("<#X1N:%s;%s;%c;->\n", relay_addr[rindx].unit,relay_addr[rindx].relay, value);
    SerialRfm.printf("<#X1N:%s;%s;%c;->\n", relay_addr[rindx].unit,relay_addr[rindx].relay, value);
}

const relay_addr_st *relay_get_addr_ptr( va_relays_et relay_id)
{
    return &relay_addr[relay_id];
}

bool relay_get_is_relay_in_group(va_relays_et rindx, uint8_t gindx )
{
    return ((relay_groups[gindx] & relay_addr[rindx].group_map) != 0);
}


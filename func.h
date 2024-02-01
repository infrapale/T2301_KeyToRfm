#ifndef __FUNC_H__
#define __FUNC_H__

#include <Arduino.h>
#include "kbd_uart.h"

#define NBR_KEYS_PER_PAD    8
#define NBR_KEY_PADS        2

typedef enum
{
    FUNC_UNDEFINED = 0,
    FUNC_RELAY,
    FUNC_RELAY_GROUP,
    FUNC_OPTION,
    FUNC_NBR_OF
} func_et;


typedef struct
{
    func_et  type;
    uint8_t  indx;

} key_function_st;

bool func_get_key(kbd_data_st *pkey, key_function_st *pfunc);

#endif


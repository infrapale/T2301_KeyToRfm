#ifndef __FUNC_H__
#define __FUNC_H__

#include <Arduino.h>

#define NBR_KEYS_PER_PAD    8
#define NBR_KEY_PADS        2

typedef enum
{
    FUNC_UNDEFINED = 0,
    FUNC_RELAY,
    FUNC_RELAY_GROUP,
    FUNC_NBR_OF
} func_et;


typedef struct
{
    func_et  func_type;
    uint8_t   relay_indx;

} key_function_st;

#endif
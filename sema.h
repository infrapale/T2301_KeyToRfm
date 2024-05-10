#ifndef __SEMA_H__
#define __SEMA_H__
#include "Arduino.h"

#define SEMA_NBR_OF   8

bool sema_reserve( uint8_t sema_indx);

void sema_release(uint8_t sema_indx);

void sema_status_print(void);
#endif
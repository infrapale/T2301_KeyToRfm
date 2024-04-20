#ifndef __MAIN_H__
#define __MAIN_H__

#include "Arduino.h"
#define APP_NAME "T2401_KeyToRfm"
#define RFM_SEND_INTERVAL 2000

#define SerialKbd   Serial1
#define SerialRfm   Serial2
#define SerialClock Serial2
#define PIN_NEO_PIXEL 22
#define PIN_PIR       15
#define PIN_LDR       26

typedef enum {
  TASK_READ_KEY = 0,
  TASK_SEND_RFM,
  TASK_MENU_TIMEOUT,
  TASK_SIGNAL,
  TASK_SIGNAL_STATE,
  TASK_AUTOM,
  TASK_DEBUG,
  TASK_NBR_OF
} tasks_et;

typedef enum {
  SEMA_SERIAL2 = 0,
  SEMA_XXX
} sema_et;


typedef enum {
  STATUS_AT_HOME = 0,
  STATUS_AWAY
} status_et;

typedef struct
{
  uint8_t hour;
  uint8_t minute;
} time_st;


typedef struct
{
  time_st time;
  uint16_t light;
} main_ctrl_st;

#endif
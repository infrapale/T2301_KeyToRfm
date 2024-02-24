#ifndef __MAIN_H__
#define __MAIN_H__

#include "Arduino.h"
#define APP_NAME   "T2401_KeyToRfm"
#define RFM_SEND_INTERVAL  2000

#define SerialKbd  Serial1
#define SerialRfm  Serial2

typedef enum
{
    TASK_READ_KEY = 0,
    TASK_SEND_RFM,
    TASK_UPDATE_CLOCK24,
    TASK_NBR_OF
} tasks_et;

typedef enum
{
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
  time_st     time;
  status_et   status;
  uint16_t    light;
} main_ctrl_st;

#endif
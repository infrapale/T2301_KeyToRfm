#ifndef __MAIN_H__
#define __MAIN_H__

#include "Arduino.h"
#define APP_NAME "T2401_KeyToRfm"
#define RFM_SEND_INTERVAL 2000

// #define COMBO_2403

#define PIN_SERIAL1_TX (0u)
#define PIN_SERIAL1_RX (1u)

#undef  PIN_SERIAL2_TX 
#undef  PIN_SERIAL2_RX

#define PIN_SERIAL_2_TX (4u)
#define PIN_SERIAL_2_RX (5u)



#define SerialKbd   Serial1
#define SerialRfm   Serial2
#define SerialClock Serial2
#define PIN_NEO_PIXEL 22
#define PIN_PIR       15
#define PIN_LDR       26

#ifdef  COMBO_2403
#define PIN_PWR_0       (3u)  
#define PIN_PWR_1       (2u)  
#endif

// I2C PIN Definitions
#ifdef  COMBO_2403
#define PIN_I2C_SCL     (5u)  
#define PIN_I2C_SDA     (4u)
#else
#define PIN_I2C_SCL     (9u)  
#define PIN_I2C_SDA     (8u)
#define PIN_I2C1_SCL    (11u)  
#define PIN_I2C1_SDA    (10u)
#define PIN_WD_RESET    (13u)
#endif



typedef enum {
  TASK_READ_KEY = 0,
  TASK_SEND_RFM,
  TASK_MENU_TIMEOUT,
  TASK_SIGNAL,
  TASK_SIGNAL_STATE,
  TASK_AUTOM,
  TASK_SUPERVISOR,
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
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} time_st;


typedef struct
{
  time_st time;
  uint16_t light;
} main_ctrl_st;

typedef struct
{
  uint16_t main_state;
  uint16_t restart_cntr;

} main_eeprom_data_st;
#endif
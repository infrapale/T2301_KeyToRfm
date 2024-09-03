#ifndef __AUTOM_H__
#define __AUTOM_H__

typedef enum
{
  RELAY_PROG_UNDEF    = 0x00,
  RELAY_PROG_AT_HOME  = 0x01,
  RELAY_PROG_AWAY     = 0x02,
  RELAY_PROG_ALARM    = 0x04,
  RELAY_PROG_WARNING  = 0x08,
  RELAY_PROG_5        = 0x10,
  RELAY_PROG_6        = 0x20,
  RELAY_PROG_7        = 0x40,
  RELAY_PROG_8        = 0x80,
} relay_prog_et;

typedef enum
{
  JANUARY = 0,
  FEBRUARY,
  MARCH,
  APRIL,
  MAY,
  JUNE,
  JULY,
  AUGUST,
  SEPTEMBER,
  OCTOBER,
  NOVEMBER,
  DECEMBER
} month_et;

void autom_initialize(uint8_t hour, uint8_t minute);

void autom_set_program(relay_prog_et  program);

uint8_t autom_get_program(void);

void autom_randomize(void);

void autom_task();

uint8_t autom_get_hour(void);

uint8_t autom_get_minute(void);

void autom_set_time(void);

#endif

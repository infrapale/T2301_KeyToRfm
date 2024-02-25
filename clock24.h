#ifndef __CLOCK24_H__
#define __CLOCK24_H__

typedef enum 
{
  CLOCK_STATE_AT_HOME = 0x0001UL,
  CLOCK_STATE_SENDING = 0x0002UL,
  CLOCK_STATE_OPTION  = 0x0004UL,
  CLOCK_STATE_CNTDWN  = 0x0008UL
} clock_state_et;


void clock24_initialize(void);

void clock24_show_task(void);

void clock24_set_time(uint8_t hour, uint8_t minute);

uint16_t clock24_get_state(void);

void clock24_set_state(clock_state_et c_state );

void clock24_clear_state(clock_state_et c_state );

#endif
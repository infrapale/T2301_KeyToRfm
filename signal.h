#ifndef __SIGNAL_H__
#define __SIGNAL_H__
#define NBR_COLOR_SEQ   3

typedef enum
{
  RGB_BLACK   = ((0x00 << 16) | (0x00 << 8) | 0x00),
  RGB_RED     = ((0xff << 16) | (0x00 << 8) | 0x00),
  RGB_GREEN   = ((0x00 << 16) | (0xff << 8) | 0x00),
  RGB_BLUE    = ((0x00 << 16) | (0x00 << 8) | 0xff),
  RGB_WHITE   = ((0xff << 16) | (0xff << 8) | 0xff),
  RGB_MAGENTA = ((0xff << 16) | (0x00 << 8) | 0xff),
  RGB_CYAN    = ((0x00 << 16) | (0xff << 8) | 0xff),
  RGB_YELLOW  = ((0xff << 16) | (0xFF << 8) | 0x00),
} rgb_colors_et;

typedef enum
{
  RGB_INDX_BLACK = 0,
  RGB_INDX_RED ,
  RGB_INDX_GREEN,
  RGB_INDX_BLUE,
  RGB_INDX_WHITE,
  RGB_INDX_MAGENTA,
  RGB_INDX_CYAN,
  RGB_INDX_YELLOW,
  RGB_INDX_NBR_OF
} rgb_color_indx_et;

typedef enum
{
  SIGNAL_STATE_START     = 0x00,
  SIGNAL_STATE_AT_HOME   = 0x10,
  SIGNAL_STATE_COUNTDOWN = 0x20,
  SIGNAL_STATE_AWAY      = 0x30,
  SIGNAL_STATE_WARNING   = 0x40,
  SIGNAL_STATE_ALARM     = 0x50,
  SIGNAL_STATE_SENDING   = 0x60,
} signal_state_et;

typedef enum
{
  SIGNAL_INDEX_START = 0,
  SIGNAL_INDEX_AT_HOME,
  SIGNAL_INDEX_COUNTDOWN,
  SIGNAL_INDEX_AWAY,
  SIGNAL_INDEX_WARNING,
  SIGNAL_INDEX_ALARM,
  SIGNAL_INDEX_SENDING,
  SIGNAL_INDEX_NBR_OF
} signal_index_et;

typedef enum
{
  SIGNAL_EVENT_UNDEFINED = 0,
  SIGNAL_EVENT_LOGIN,
  SIGNAL_EVENT_LOGOUT,
  SIGNAL_EVENT_LEAVE,
  SIGNAL_EVENT_ALERT,
  SIGNAL_EVENT_CONFIRM,
  SIGNAL_EVENT_SENDING,
  SIGNAL_EVENT_TIMEOUT,
  SIGNAL_EVENT_NBR_OF
} signal_event_et;

typedef struct
{
  uint8_t color;
  uint8_t duration;
} signal_pattern_st;

typedef signal_pattern_st signal_seq[NBR_COLOR_SEQ];

void signal_initialize(void);

void signal_update(void);

void signal_set_event(signal_event_et signal_event);

//void signal_set_state(signal_state_et new_state);

uint16_t signal_get_state(void);

void signal_set_state(uint16_t new_state);

uint8_t signal_get_state_index(void);

char *signal_get_state_label(void);

//void signal_return_state(void);

void signal_state_machine(void);


#endif
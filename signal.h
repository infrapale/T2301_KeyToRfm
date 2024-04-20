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
  SIGNAL_START = 0,
  SIGNAL_AT_HOME,
  SIGNAL_COUNTDOWN,
  SIGNAL_AWAY,
  SIGNAL_WARNING,
  SIGNAL_ALARM,
  SIGNAL_SENDING,
  SIGNAL_NBR_OF
} signal_state_et;


typedef struct
{
  uint8_t color;
  uint8_t duration;
} signal_pattern_st;

typedef signal_pattern_st signal_seq[NBR_COLOR_SEQ];

void signal_initialize(void);

void signal_update(void);

void signal_set_state(signal_state_et new_state);

signal_state_et signal_get_state(void);

void signal_return_state(void);


#endif
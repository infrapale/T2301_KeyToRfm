#ifndef __MENU4X2_H__
#define __MENU4X2_H__

#define LABEL_LEN 10
#define MENU_ROWS 4
#define MENU_COLS 2
#define MENU_TOTAL  (MENU_ROWS * MENU_COLS)

typedef enum
{
  MENU_ROOT = 0,
  MENU_OPTION,
  MENU_SET_TIME,
  MENU_SET_DATE,
  MENU_HOME,
  MENU_INFO,
  MENU_INFO_1,
  MENU_INFO_2,
  MENU_INFO_3,
  MENU_CODE_1,
  MENU_CODE_2,
  MENU_TEST,
  MENU_NBR_OF  
} menu_index_et;

typedef enum
{
  MENU_CAT_EMPTY = 0,
  MENU_CAT_ACTIVE,
  MENU_CAT_SENSOR,
  MENU_CAT_TITLE,
  MENU_CAT_STATE,
  MENU_CAT_DATE_TIME,
  MENU_CAT_RESTARTS,
} menu_category_et;

typedef void (*menu_cb)(void);

typedef struct 
{
  char      label[LABEL_LEN +1];
  uint8_t   category;
  uint8_t   next_level;
  menu_cb   cb;
}  menu_item_st;

typedef menu_item_st menu4x2_t[MENU_TOTAL];    //[MENU_NBR_OF];

void menu4x2_initialize(void);

void menu4x2_show(uint8_t mindx);

bool menu4x2_key_do_menu(char key);

void menu4x2_key_pressed(char key);

void menu4x2_show_now(void);

void menu4x2_timeout_task(void);

#endif
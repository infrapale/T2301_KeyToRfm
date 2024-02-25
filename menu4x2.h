#ifndef __MENU4X2_H__
#define __MENU4X2_H__

#define LABEL_LEN 10
#define MENU_ROWS 4
#define MENU_COLS 2
#define MENU_TOTAL  (MENU_ROWS * MENU_COLS)

typedef enum
{
  MENU_MAIN = 0,
  MENU_OPTION,
  MENU_NBR_OF  
} menu_index_et;

typedef void (*menu_cb)(void);

typedef struct 
{
  char      label[LABEL_LEN +1];
  uint8_t   next_menu_indx;
  menu_cb   cb;
}  menu_item_st;


typedef menu_item_st menu4x2_t[MENU_TOTAL];    //[MENU_NBR_OF];

void menu4x2_show(uint8_t mindx);

#endif
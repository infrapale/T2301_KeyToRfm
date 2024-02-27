#include "Arduino.h"
#include "main.h"
#include <LiquidCrystal_PCF8574.h>
#include "menu4x2.h"


typedef struct
{
  uint8_t row;
  uint8_t col;
} menu_def_st;

typedef struct 
{
   uint8_t level;
} menu4x2_ctrl_st;

extern LiquidCrystal_PCF8574 lcd;
extern main_ctrl_st main_ctrl;

void dummy_menu(void)
{

};

void hour_plus(void)
{
    if(++main_ctrl.time.hour > 23) main_ctrl.time.hour = 0;
}
void hour_minus(void)
{
    if(main_ctrl.time.hour > 0) main_ctrl.time.hour = 23;
}

void minute_plus(void)
{
    if(++main_ctrl.time.minute > 59) main_ctrl.time.minute = 0;
}
void minute_minus(void)
{
    if(main_ctrl.time.minute > 0) main_ctrl.time.minute--;
    else  main_ctrl.time.minute = 59;
}

menu4x2_ctrl_st menu4x2_ctrl;


const menu_def_st menu4x2_def[MENU_TOTAL] =
{
  {0 ,10},
  {1 ,10},
  {2 ,10},
  {3 ,10},
  {0 ,0},
  {1 ,0},
  {2 ,0},
  {3 ,0},
};

menu4x2_t menu4x2[MENU_NBR_OF] =
{
  [MENU_MAIN] =
  {
    { "12:33     ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "Valitse   ", 1, MENU_OPTION, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "Kotona?   ", 1, MENU_HOME, dummy_menu}
  },
  [MENU_OPTION] =
  {
    { "Set Time  ", 1, MENU_SET_TIME, dummy_menu},
    { "          ", 0, MENU_HOME, dummy_menu},
    { "All Off   ", 0, MENU_MAIN, dummy_menu},
    { "Takaisin  ", 1, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu}
  },
  [MENU_SET_TIME] =
  {
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "Tunti+1   ", 1, MENU_SET_TIME, hour_plus},
    { "Tunti-1   ", 1, MENU_SET_TIME, hour_minus},
    { "Alkuun    ", 1, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "Minuutti+ ", 1, MENU_SET_TIME, minute_plus},
    { "Minuutti- ", 1, MENU_SET_TIME, minute_minus},
    { "          ", 0, MENU_OPTION, dummy_menu},
  },
  [MENU_HOME] =
  {
    { "Kotona    ", 1, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "Alkuun    ", 1, MENU_MAIN, dummy_menu},
    { "Poissa    ", 1, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu},
    { "          ", 0, MENU_MAIN, dummy_menu}
  },
};

void menu4x2_initialize(void)
{
  menu4x2_ctrl.level = MENU_MAIN;
  menu4x2_show(MENU_MAIN);

}


void menu4x2_show(uint8_t mindx)
{
    char line0[21];
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();


    for (uint8_t i = 0; i < MENU_TOTAL; i++)
    {
      if ((i==4))  // && (mindx == MENU_MAIN))
      {
          lcd.setCursor(0,0);
          sprintf(line0, "%02d:%02d", main_ctrl.time.hour, main_ctrl.time.minute);
          lcd.print (line0);
          Serial.println(line0);
          // lcd.print(main_ctrl.time.hour);
          // lcd.print(" :");
          // lcd.print(main_ctrl.time.minute);
      }



      else
      {
        lcd.setCursor(menu4x2_def[i].col, menu4x2_def[i].row);
        lcd.print(menu4x2[mindx][i].label);
      }
    }
}


bool menu4x2_key_do_menu(char key)
{
  bool do_menu = false;
  uint8_t ikey = key - '1';
  if (ikey < MENU_TOTAL)
  {
    if (menu4x2[menu4x2_ctrl.level][ikey].active > 0) do_menu = true;
  }
  return do_menu;
}


void menu4x2_key_pressed(char key)
{
  uint8_t ikey = key - '1';
  uint8_t next_menu;
  if (ikey < MENU_TOTAL)
  {
      next_menu = menu4x2[menu4x2_ctrl.level][ikey].next_level;
      menu4x2[menu4x2_ctrl.level][ikey].cb();
      menu4x2_ctrl.level = next_menu;
      menu4x2_show(menu4x2_ctrl.level);
  }


}

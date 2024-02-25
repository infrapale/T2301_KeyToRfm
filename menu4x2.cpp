#include "Arduino.h"
#include <LiquidCrystal_PCF8574.h>
#include "menu4x2.h"


typedef struct
{
  uint8_t row;
  uint8_t col;
} menu_def_st;

void dummy_menu(void)
{

};

void menu4x2_initialize(void)
{

}

extern LiquidCrystal_PCF8574 lcd;

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
  {
    { "Menu1     ", MENU_MAIN, dummy_menu},
    { "Menu2     ", MENU_MAIN, dummy_menu},
    { "Menu3     ", MENU_MAIN, dummy_menu},
    { "Option    ", MENU_OPTION, dummy_menu},
    { "Menu5     ", MENU_MAIN, dummy_menu},
    { "Menu6     ", MENU_MAIN, dummy_menu},
    { "Menu7     ", MENU_MAIN, dummy_menu},
    { "Menu8     ", MENU_MAIN, dummy_menu}
  },
  {
    { "Set Time  ", MENU_MAIN, dummy_menu},
    { "Menu2     ", MENU_MAIN, dummy_menu},
    { "Menu3     ", MENU_MAIN, dummy_menu},
    { "Menu4     ", MENU_MAIN, dummy_menu},
    { "Menu5     ", MENU_MAIN, dummy_menu},
    { "Menu6     ", MENU_MAIN, dummy_menu},
    { "Menu7     ", MENU_MAIN, dummy_menu},
    { "Menu8     ", MENU_MAIN, dummy_menu}
  }
};


void menu4x2_show(uint8_t mindx)
{
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.print("Hello LCD");
    for (uint8_t i = 0; i < MENU_TOTAL; i++)
    {
      lcd.setCursor(menu4x2_def[i].col, menu4x2_def[i].row);
      lcd.print(menu4x2[mindx][i].label);
    }
}

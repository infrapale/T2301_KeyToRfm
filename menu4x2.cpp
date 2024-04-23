#include "Arduino.h"
#include "main.h"
#include <LiquidCrystal_PCF8574.h>
#include "menu4x2.h"
#include "signal.h"

#define TIMEOUT_MENU         10000
#define TIMEOUT_BACK_LIGHT   30000

typedef struct
{
  uint8_t row;
  uint8_t col;
} menu_def_st;

typedef struct 
{
   uint8_t  level;
   uint32_t bl_timeout_at;
   uint32_t menu_timeout_at;
} menu4x2_ctrl_st;

extern LiquidCrystal_PCF8574 lcd;
extern main_ctrl_st main_ctrl;

menu4x2_ctrl_st menu4x2_ctrl;

void dummy_menu(void)
{

};

void hour_plus(void)
{
    if(++main_ctrl.time.hour > 23) main_ctrl.time.hour = 0;
}
void hour_minus(void)
{
    if(main_ctrl.time.hour > 0) main_ctrl.time.hour--;
    else main_ctrl.time.hour = 23;
}

void minute_plus_10(void)
{
    main_ctrl.time.minute +=  10;
    if(main_ctrl.time.minute > 59) main_ctrl.time.minute -= 60;
}
void minute_plus_1(void)
{
    if(++main_ctrl.time.minute > 59) main_ctrl.time.minute -=60;
}
 

void send_signal_event_alert(void)
{
   signal_set_event(SIGNAL_EVENT_ALERT);
}

void send_signal_event_leave(void)
{
   signal_set_event(SIGNAL_EVENT_LEAVE);
}

void send_signal_event_login(void)
{
   signal_set_event(SIGNAL_EVENT_LOGIN);
}

void send_signal_event_confirm(void)
{
   signal_set_event(SIGNAL_EVENT_CONFIRM);
}


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
    { "          ", MENU_CAT_EMPTY     , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_MAIN, dummy_menu},
    { "Valitse   ", MENU_CAT_ACTIVE    , MENU_OPTION, dummy_menu},
    { "          ", MENU_CAT_TITLE     , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_SHOW_TIME , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_STATE     , MENU_MAIN, dummy_menu},
    { "Test      ", MENU_CAT_ACTIVE    , MENU_TEST, dummy_menu}
  },
  [MENU_OPTION] =
  {
    { "Set Time  ", MENU_CAT_ACTIVE    , MENU_SET_TIME, dummy_menu},
    { "Kotona    ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_login},
    { "All Off   ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Alkuun    ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Kuittaa   ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_confirm},
    { "Poissa    ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_leave},
    { "          ", MENU_CAT_EMPTY     , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_MAIN, dummy_menu}
  },
  [MENU_SET_TIME] =
  {
    { "          ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Min + 10  ", MENU_CAT_ACTIVE    , MENU_SET_TIME, minute_plus_10},
    { "Min + 1   ", MENU_CAT_ACTIVE    , MENU_SET_TIME, minute_plus_1},
    { "Alkuun    ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_SHOW_TIME , MENU_SET_TIME, dummy_menu},
    { "Tunti+1   ", MENU_CAT_ACTIVE    , MENU_SET_TIME, hour_plus},
    { "Tunti-1   ", MENU_CAT_ACTIVE    , MENU_SET_TIME, hour_minus},
    { "Hyvaksy   ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
  },
  [MENU_HOME] =
  {
    { "          ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Kotona    ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_login},
    { "          ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Alkuun    ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Poissa    ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_leave},
    { "          ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu}
  },
  [MENU_TEST] =
  {
    { "          ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Kotona    ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Alkuun    ", MENU_CAT_ACTIVE    , MENU_MAIN, dummy_menu},
    { "Login     ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_login},
    { "Leave     ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_leave},
    { "Alert     ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_alert},
    { "Confirm   ", MENU_CAT_ACTIVE    , MENU_MAIN, send_signal_event_confirm}
  },
};


void menu4x2_reset_timeout(void)
{
  menu4x2_ctrl.menu_timeout_at = millis() + TIMEOUT_MENU;
  menu4x2_ctrl.bl_timeout_at = millis() + TIMEOUT_BACK_LIGHT;
}

void menu4x2_initialize(void)
{
  menu4x2_ctrl.level = MENU_MAIN;
  menu4x2_reset_timeout();
  lcd.setBacklight(1);
  menu4x2_show(MENU_MAIN);

}


void menu4x2_show(uint8_t mindx)
{
    char line0[21];
    //lcd.setBacklight(1);
    lcd.home();
    lcd.clear();


    for (uint8_t i = 0; i < MENU_TOTAL; i++)
    {
      switch( menu4x2[menu4x2_ctrl.level][i].category )
      {
        case MENU_CAT_EMPTY:
          break;
        case MENU_CAT_ACTIVE:
          lcd.setCursor(menu4x2_def[i].col, menu4x2_def[i].row);
          lcd.print(menu4x2[mindx][i].label);
          break;  
        case MENU_CAT_SHOW_TIME:
          lcd.setCursor(menu4x2_def[i].col, menu4x2_def[i].row);
          //lcd.setCursor(0,0);
          sprintf(line0, "%02d:%02d", main_ctrl.time.hour, main_ctrl.time.minute);
          lcd.print (line0);
          Serial.println(line0);
          break;
        case MENU_CAT_TITLE:
          lcd.setCursor(menu4x2_def[i].col, menu4x2_def[i].row);
          sprintf(line0, "Villa Astrid");
          lcd.print (line0);
          Serial.println(line0);
          break;
        case MENU_CAT_STATE:
          lcd.setCursor(menu4x2_def[i].col, menu4x2_def[i].row);
          sprintf(line0, "%s", signal_get_state_label());
          lcd.print (line0);
          Serial.println(line0);
          break;
      }
    }
}


bool menu4x2_key_do_menu(char key)
{
  bool do_menu = false;
  uint8_t ikey = key - '1';
  if (ikey < MENU_TOTAL)
  {
    if (menu4x2[menu4x2_ctrl.level][ikey].category >= MENU_CAT_ACTIVE) do_menu = true;
  }
  return do_menu;
}


void menu4x2_key_pressed(char key)
{
  uint8_t ikey = key - '1';
  uint8_t next_menu;
  if (ikey < MENU_TOTAL)
  {
      menu4x2_reset_timeout();
      lcd.setBacklight(1);

      next_menu = menu4x2[menu4x2_ctrl.level][ikey].next_level;
      menu4x2[menu4x2_ctrl.level][ikey].cb();
      menu4x2_ctrl.level = next_menu;
      menu4x2_show(menu4x2_ctrl.level);
  }
}

void menu4x2_timeout_task(void)
{
    if (menu4x2_ctrl.menu_timeout_at < millis())
    {
      menu4x2_ctrl.level = MENU_MAIN;
      menu4x2_show(menu4x2_ctrl.level);
      menu4x2_ctrl.menu_timeout_at = millis() + TIMEOUT_MENU;
    } 

    if (menu4x2_ctrl.bl_timeout_at < millis())
    {
      lcd.setBacklight(0);
      menu4x2_ctrl.bl_timeout_at = millis() + TIMEOUT_BACK_LIGHT;
    }

    if (digitalRead(PIN_PIR) == HIGH)
    {
      menu4x2_ctrl.bl_timeout_at = millis() + TIMEOUT_BACK_LIGHT;
      lcd.setBacklight(1);
    }

}


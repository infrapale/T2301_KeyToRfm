#include "Arduino.h"
#include "main.h"
#include <LiquidCrystal_PCF8574.h>
#include "menu4x2.h"
#include "signal.h"
#include "autom.h"
#include "supervisor.h"

#define TIMEOUT_MENU         10000
#define TIMEOUT_UPDATE       1000
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
   uint32_t menu_update_at;
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

void month_plus_1(void)
{
    if(++main_ctrl.time.month > 12) main_ctrl.time.month = 1;
}

void month_minus_1(void)
{
    if (main_ctrl.time.month > 1) main_ctrl.time.month--;
    else main_ctrl.time.month = 12;
}
void day_plus_1(void)
{
    if(++main_ctrl.time.day > 31) main_ctrl.time.day = 1;
}

void day_minus_1(void)
{
    if (main_ctrl.time.day > 1) main_ctrl.time.day--;
    else main_ctrl.time.day = 31;
}


void send_signal_event_alert(void)
{
   signal_set_event(SIGNAL_EVENT_ALERT);
   menu4x2_show_now();
}

void send_signal_event_leave(void)
{
   signal_set_event(SIGNAL_EVENT_LEAVE);
   menu4x2_show_now();
}

void send_signal_event_login(void)
{
   signal_set_event(SIGNAL_EVENT_LOGIN);
   menu4x2_show_now();
}

void send_signal_event_confirm(void)
{
   signal_set_event(SIGNAL_EVENT_CONFIRM);
   menu4x2_show_now();
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

/********************************************************************************************
    Menu definition indeces     Fixed Labels
    --------      --------
    |  4   |      |  0   |      Menu    Cancel
    --------      --------
    |  5   |      |  1   |      
    --------      --------
    |  6   |      |  2   |
    --------      --------
    |  7   |      |  3   |
    --------      --------
*********************************************************************************************
  Log in at home  
    (root)[4]->(option)[4]->()[4]->()[7]->()[10sec]->(root)
  Log out away  
    (root)[4]->(option)[4]->()[4]->()[3]->()[10sec]->(root)
  Set Date
    (root)[4]->(option)[5]->(date)[5]->()[10sec]->(root)   month + 1
    (root)[4]->(option)[5]->(date)[6]->()[10sec]->(root)   month - 1
    (root)[4]->(option)[5]->(date)[1]->()[10sec]->(root)   day + 1
    (root)[4]->(option)[5]->(date)[5]->()[10sec]->(root)   day - 1
  Set Time
    (root)[4]->(option)[1]->(date)[5]->()[10sec]->(root)   hour + 1
    (root)[4]->(option)[1]->(date)[6]->()[10sec]->(root)   hour - 1
    (root)[4]->(option)[1]->(date)[1]->()[10sec]->(root)   minute + 1
    (root)[4]->(option)[1]->(date)[5]->()[10sec]->(root)   minute - 1
  Info
    (root)[4]->(option)[6]->(date)[5]->[30sec]->(root)     show info 1
    (root)[4]->(option)[5]->(date)[6]->()[10sec]->(root)   show info 2
    (root)[4]->(option)[5]->(date)[1]->()[10sec]->(root)   show info 3

    Info 1:
      Restarts  nn
      LDR nn PIR n
      Status text
    Info 2:
      TBD
    Info 3:

  

********************************************************************************************/

menu4x2_t menu4x2[MENU_NBR_OF] =
{
  [MENU_ROOT] =
  {
    { "          ", MENU_CAT_EMPTY     , MENU_OPTION, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_TITLE     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_DATE_TIME , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_STATE     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu}
  },
  [MENU_OPTION] =
  {
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "Aika =    ", MENU_CAT_ACTIVE    , MENU_SET_TIME, dummy_menu},
    { "All Off   ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Test      ", MENU_CAT_ACTIVE    , MENU_TEST, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_CODE_1, dummy_menu},
    { "Paivam =  ", MENU_CAT_ACTIVE    , MENU_SET_DATE, dummy_menu},
    { "Info      ", MENU_CAT_ACTIVE    , MENU_INFO, dummy_menu},
    { "Kuittaa   ", MENU_CAT_EMPTY     , MENU_ROOT, send_signal_event_confirm},
  },
  [MENU_SET_TIME] =
  {
    { "          ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Min + 10  ", MENU_CAT_ACTIVE    , MENU_SET_TIME, minute_plus_10},
    { "Min + 1   ", MENU_CAT_ACTIVE    , MENU_SET_TIME, minute_plus_1},
    { "Alkuun    ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_DATE_TIME , MENU_SET_TIME, dummy_menu},
    { "Tunti+1   ", MENU_CAT_ACTIVE    , MENU_SET_TIME, hour_plus},
    { "Tunti-1   ", MENU_CAT_ACTIVE    , MENU_SET_TIME, hour_minus},
    { "Hyvaksy   ", MENU_CAT_ACTIVE    , MENU_ROOT, autom_set_time},
  },
  [MENU_SET_DATE] =
  {
    { "          ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Paiva + 1 ", MENU_CAT_ACTIVE    , MENU_SET_DATE, day_plus_1},
    { "Paiva - 1 ", MENU_CAT_ACTIVE    , MENU_SET_DATE, day_minus_1},
    { "Alkuun    ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_DATE_TIME , MENU_SET_DATE, dummy_menu},
    { "KK + 1    ", MENU_CAT_ACTIVE    , MENU_SET_DATE, month_plus_1},
    { "KK - 1    ", MENU_CAT_ACTIVE    , MENU_SET_DATE, month_minus_1},
    { "Hyvaksy   ", MENU_CAT_ACTIVE    , MENU_OPTION, autom_set_time},
  },
  [MENU_HOME] =
  {
    { "          ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Kotona    ", MENU_CAT_ACTIVE    , MENU_ROOT, send_signal_event_login},
    { "          ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Alkuun    ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Poissa    ", MENU_CAT_ACTIVE    , MENU_ROOT, send_signal_event_leave},
    { "Sammuta   ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu}
  },
  [MENU_INFO] =
  {
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "Valitse   ", MENU_CAT_ACTIVE    , MENU_OPTION, dummy_menu},
    { "          ", MENU_CAT_RESTARTS  , MENU_ROOT, dummy_menu},
    { "Info 1    ", MENU_CAT_ACTIVE    , MENU_INFO_1, dummy_menu},
    { "Info 2    ", MENU_CAT_EMPTY     , MENU_INFO_2, dummy_menu},
    { "Info 3    ", MENU_CAT_EMPTY     , MENU_INFO_3, dummy_menu}
  },
  [MENU_INFO_1] =
  {
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_OPTION, dummy_menu},
    { "          ", MENU_CAT_RESTARTS  , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_SENSOR    , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu}
  },
  [MENU_INFO_2] =
  {
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_OPTION, dummy_menu},
    { "          ", MENU_CAT_RESTARTS  , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_SENSOR    , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu}
  },
  [MENU_INFO_3] =
  {
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_OPTION, dummy_menu},
    { "          ", MENU_CAT_RESTARTS  , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_SENSOR    , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu}
  },
  [MENU_CODE_1] =
  {
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_CODE_2, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu}
  },
[MENU_CODE_2] =
  {
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, send_signal_event_leave},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_EMPTY     , MENU_ROOT, send_signal_event_login}
  },

  [MENU_TEST] =
  {
    { "          ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Kotona    ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "          ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Alkuun    ", MENU_CAT_ACTIVE    , MENU_ROOT, dummy_menu},
    { "Login     ", MENU_CAT_ACTIVE    , MENU_ROOT, send_signal_event_login},
    { "Leave     ", MENU_CAT_ACTIVE    , MENU_ROOT, send_signal_event_leave},
    { "Alert     ", MENU_CAT_ACTIVE    , MENU_ROOT, send_signal_event_alert},
    { "Confirm   ", MENU_CAT_ACTIVE    , MENU_ROOT, send_signal_event_confirm}
  },
};


void menu4x2_reset_timeout(void)
{
  menu4x2_ctrl.menu_update_at = millis() + TIMEOUT_UPDATE;
  menu4x2_ctrl.menu_timeout_at = millis() + TIMEOUT_MENU;
  menu4x2_ctrl.bl_timeout_at = millis() + TIMEOUT_BACK_LIGHT;
}

void menu4x2_initialize(void)
{
  menu4x2_ctrl.level = MENU_ROOT;
  menu4x2_reset_timeout();
  lcd.setBacklight(1);
  menu4x2_show(MENU_ROOT);

}


void menu4x2_show(uint8_t mindx)
{
    char line0[40];
    //lcd.setBacklight(1);
    lcd.home();
    lcd.clear();


    for (uint8_t i = 0; i < MENU_TOTAL; i++)
    {
      lcd.setCursor(menu4x2_def[i].col, menu4x2_def[i].row);
      switch( menu4x2[menu4x2_ctrl.level][i].category )
      {
        case MENU_CAT_EMPTY:
          break;
        case MENU_CAT_ACTIVE:
          lcd.print(menu4x2[mindx][i].label);
          break;  
        case MENU_CAT_SENSOR:
          sprintf(line0, "LDR: %4d PIR: %1d", supervisor_get_ldr(), supervisor_get_pir());
          lcd.print (line0);
          // Serial.println(line0);
          break;
        case MENU_CAT_TITLE:
          sprintf(line0, "*** Villa Astrid ***");
          lcd.print (line0);
          // Serial.println(line0);
          break;
        case MENU_CAT_STATE:
          sprintf(line0, "%s %02X %d", signal_get_state_label(), signal_get_cntr(), autom_get_program());
          //sprintf(line0, "%s %02X %d", "abc", signal_get_state(), autom_get_program());
          //sprintf(line0, "%02X %d", signal_get_state(), autom_get_program());
          lcd.print (line0);
          // Serial.println(line0);
          break;
        case MENU_CAT_DATE_TIME:
          sprintf(line0, "%02d-%02d-%02d %02d:%02d",
              main_ctrl.time.year,
              main_ctrl.time.month, 
              main_ctrl.time.day, 
              main_ctrl.time.hour, 
              main_ctrl.time.minute);
          lcd.print (line0);
          // Serial.println(line0);
          break;
        case MENU_CAT_RESTARTS:
          sprintf(line0, "Restarts: %5d", main_ctrl.restart_cntr);
          lcd.print (line0);
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

void menu4x2_show_now(void)
{
    menu4x2_ctrl.menu_update_at = millis() - 1;
}

void menu4x2_timeout_task(void)
{
    if (menu4x2_ctrl.menu_update_at < millis())
    {
      menu4x2_show(menu4x2_ctrl.level);
      menu4x2_ctrl.menu_update_at = millis() + TIMEOUT_UPDATE;
    } 

    if (menu4x2_ctrl.menu_timeout_at < millis())
    {
      menu4x2_ctrl.level = MENU_ROOT;
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


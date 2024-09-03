
#include "main.h"
#include "relay.h"
#include "task.h"
#include "autom.h"
#include "signal.h"
#include "sema.h"
#include "supervisor.h"

#define MONTHS_PER_YEAR   12
#define HOURS_PER_DAY     24
#define MINUTES_PER_HOUR  60
#define MINUTES_PER_DAY   (HOURS_PER_DAY * MINUTES_PER_HOUR)
#define NEXT_RANDOM_ITER  600

typedef enum
{
    RELAY_AUTOM_STATE_UNDEF = 0,
    RELAY_AUTOM_STATE_OFF,
    RELAY_AUTOM_STATE_ON
} relay_autom_state_et;

typedef struct
{
   relay_prog_et  program;
   task_st  *th;
   time_st  last_time;
   uint8_t  prev_state_index;
   bool     set_time;
   uint8_t  relay_indx;
   uint32_t iter_cntr;
   uint32_t next_random;
   uint32_t next_get_time;

} autom_cntrl_st;

typedef struct
{
    uint8_t hindx;
    relay_autom_state_et autom_state;
} autom_relay_st;

extern main_ctrl_st main_ctrl;

autom_cntrl_st autom_cntrl;

String str;

char week_day[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

datetime_t rtc_time;

const uint8_t auto_prog[VA_RELAY_NBR_OF][HOURS_PER_DAY]=
{   // relay                   00    01    02    03    04    05    06    07    08    09    10    11    12    13    14    15    16    17    18    19    20    21    22    23
    [VA_RELAY_UNDEF]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_TK]         = { 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00 },
    [VA_RELAY_ET]         = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_WC_1]       = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_WC_2]       = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00 },
    [VA_RELAY_TUPA_1]     = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 },
    [VA_RELAY_TUPA_2]     = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_MH1_1]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_MH1_2]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00 },
    [VA_RELAY_MH1_3]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_MH1_4]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_MH1_5]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00 },
    [VA_RELAY_MH2_1]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_MH2_2]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_KEITTIO_1]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_KEITTIO_2]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_KEITTIO_3]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_KEITTIO_4]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_KEITTIO_5]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_KHH_1]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_KHH_2]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_PSH]        = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_SAUNA]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_PARVI]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_PIHA]       = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_TERASSI]    = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    [VA_RELAY_POLKU]      = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};


const uint32_t dark_time[MONTHS_PER_YEAR] =
{   
    //                --------222211111111110000000000
    //                --------321098765432109876543210
    [JANUARY]    = {0b00000000111111110000000111111111},  
    [FEBRUARY]   = {0b00000000111111100000000011111111},
    [MARCH]      = {0b00000000111110000000000001111111},
    [APRIL]      = {0b00000000111100000000000000111111},
    [MAY]        = {0b00000000111010101010101010101010},
    [JUNE]       = {0b00000000110000000000000000000011},
    [JULY]       = {0b00000000110000000000000000000011},
    [AUGUST]     = {0b00000000111000000000000000000111},
    [SEPTEMBER]  = {0b00000000111100000000000000001111},
    [OCTOBER]    = {0b00000000111110000000000000011111},
    [NOVEMBER]   = {0b00000000111111000000000000111111},
    [DECEMBER]   = {0b00000000111111100000000011111111},
};

 autom_relay_st autom_relay[VA_RELAY_NBR_OF];

 
void autom_initialize(uint8_t hour, uint8_t minute)
{
    autom_cntrl.last_time.hour = 0;
    autom_cntrl.last_time.minute = 0;
    autom_cntrl.program = RELAY_PROG_AT_HOME;
    autom_cntrl.relay_indx = 0;
    autom_cntrl.iter_cntr = 0;
    autom_cntrl.prev_state_index = 0;
    autom_cntrl.next_get_time = 0;
    autom_cntrl.next_random = 0;
    autom_cntrl.th = task_get_task(TASK_AUTOM);
    autom_cntrl.th->state = 0;
    autom_cntrl.set_time = false;

    for (uint8_t i = VA_RELAY_UNDEF; i < VA_RELAY_NBR_OF; i++)
    {
        autom_relay[i].hindx = 0;
        autom_relay[i].autom_state = RELAY_AUTOM_STATE_OFF;
    }
}  


void autom_set_program(relay_prog_et  program)
{
    autom_cntrl.program = program;
}

uint8_t autom_get_program(void)
{
    return (uint8_t)autom_cntrl.program;
}

void autom_randomize(void)
{
    uint16_t hm = (uint16_t)main_ctrl.time.hour * MINUTES_PER_HOUR + (uint16_t)main_ctrl.time.minute;
    randomSeed(hm);

    for (uint8_t i = VA_RELAY_UNDEF; i < VA_RELAY_NBR_OF; i++)
    {
        long l = random(-30,30);  // Serial.println(l);
        uint16_t hmx = hm + (uint16_t)l;
        if (hmx > MINUTES_PER_DAY) hmx -= MINUTES_PER_DAY;
        uint16_t hx = hmx / MINUTES_PER_HOUR;
        // Serial.println(hx);
        autom_relay[i].hindx = (uint8_t)hx;
    }
}

bool autom_on_off(uint8_t rindx)
{
    bool state_changed = false;
    uint16_t hm = (uint16_t)main_ctrl.time.hour * MINUTES_PER_HOUR + (uint16_t)main_ctrl.time.minute;
    uint8_t prog_bit  = 0x01 << autom_cntrl.program;

    uint8_t  pr = auto_prog[rindx][autom_relay[rindx].hindx];
    uint32_t dark_bit = dark_time[main_ctrl.time.month-1];
    bool     is_dark_now = (dark_bit & (1U << autom_relay[rindx].hindx)) != 0;

    Serial.printf("Dark = %08x - %d, ", dark_bit, is_dark_now);    
    Serial.printf("Relay %d hx %d on\n", rindx, autom_relay[rindx].hindx);    

    if (autom_relay[rindx].autom_state != RELAY_AUTOM_STATE_ON)
    {
        if (((pr & prog_bit) != 0) && is_dark_now)
        {
            autom_relay[rindx].autom_state = RELAY_AUTOM_STATE_ON;
            Serial.printf(" ON \n");
            state_changed = true;
        }
    } 
    else 
    {
        if (((pr & prog_bit) == 0) || !is_dark_now)
        {
            autom_relay[rindx].autom_state = RELAY_AUTOM_STATE_OFF;
            Serial.printf(" OFF \n");
            state_changed = true;
        }
    }
    return state_changed;
}


// void autom_set_time(uint8_t hour, uint8_t minute)
// {
//     autom_cntrl.hour = hour;
//     autom_cntrl.minute = minute;
// }

// uint8_t autom_get_hour(void)
// {
//     return autom_cntrl.hour;
// }

// uint8_t autom_get_minute(void)
// {
//     return autom_cntrl.minute;
// }

#define TIME_NBR_OF_FIELDS 6

bool autom_parse_time(String *tstrp)
{
    bool do_continue = true;
    uint8_t pos[TIME_NBR_OF_FIELDS + 1];
    String field[TIME_NBR_OF_FIELDS];
    // String year_str, month_str, day_str, hour_str, minute_str;

    for (uint8_t i = 0; i < TIME_NBR_OF_FIELDS+1; i++) pos[i] = 0;

    Serial.println(*tstrp);
    tstrp->trim();
    //Serial.println(tstrp->length());

    if(!tstrp->startsWith("<") || !tstrp->endsWith(">")) do_continue = false;

    if (do_continue)
    {
      if (tstrp->indexOf("C1T=") != 1) do_continue = false;
    }
    // <C1T=:2024/5/7;9:28:16>
    if (do_continue)
    {
      pos[0] = tstrp->indexOf(':');
      pos[1] = tstrp->indexOf('/',pos[0]+1);
      pos[2] = tstrp->indexOf('/',pos[1]+1);
      pos[3] = tstrp->indexOf(';',pos[2]+1);
      pos[4] = tstrp->indexOf(':',pos[3]+1);
      pos[5] = tstrp->indexOf(':',pos[4]+1);
      pos[6] = tstrp->indexOf('>',pos[5]+1);

      Serial.print("Date-Time pos: ");
      for (uint8_t i = 0; i < TIME_NBR_OF_FIELDS+1 ; i++)
      {
          if ((pos[i] == 0) || (pos[i] == 255)) do_continue = false;  
          Serial.printf("%d=%d ",i, pos[i]);
      }
      Serial.println();
    }
    //Serial.printf("ix= %d %d %d\n", i1, i2, i3);
    if (do_continue)
    {
        for (uint8_t i = 0; i < TIME_NBR_OF_FIELDS; i++)
        {
            field[i] = tstrp->substring(pos[i]+1,pos[i+1]);
        }
    }

    if (do_continue)
    {
      main_ctrl.time.year   = field[0].toInt();
      main_ctrl.time.month  = field[1].toInt();
      main_ctrl.time.day    = field[2].toInt();
      main_ctrl.time.hour   = field[3].toInt();
      main_ctrl.time.minute = field[4].toInt();
      main_ctrl.time.second = field[5].toInt();

      Serial.printf("Date-Time accepted: %04d-%02d-%02d %02d:%02d:%02d\n\r",
          main_ctrl.time.year,
          main_ctrl.time.month,
          main_ctrl.time.day,
          main_ctrl.time.hour,
          main_ctrl.time.minute,
          main_ctrl.time.second
      );
    }
    // Serial.println(do_continue);
    return do_continue;
}

void autom_set_time(void)
{
   autom_cntrl.set_time = true;
}

void autom_task()
{
    autom_cntrl.iter_cntr++;
    //Serial.printf("autom_task %d\n\r", autom_cntrl.th->state);
    switch(autom_cntrl.th->state)
    {
        case 0:  // Initial state
            if (supervisor_pwr_is_on()) 
            {
              autom_cntrl.th->state = 1;
              autom_cntrl.next_get_time = autom_cntrl.iter_cntr + 10;
            }
            break;
        case 1:
            if (autom_cntrl.iter_cntr > autom_cntrl.next_get_time) autom_cntrl.th->state = 2;
            break;
        case 2:  // Set  Mode
            if (autom_cntrl.prev_state_index != signal_get_state_index())
            {
                Serial.print("autom_task - state index: "); Serial.println(signal_get_state_index());
                if (sema_reserve( SEMA_SERIAL2))
                {
                    autom_cntrl.prev_state_index = signal_get_state_index();
                    SerialClock.printf("<C1MS:%d>\r\n", autom_cntrl.prev_state_index);
                    autom_cntrl.th->state++;
                    sema_release( SEMA_SERIAL2);
                }
                else
                {
                  Serial.print("autom_task - state index: ");
                }
            }
            else
            {
              autom_cntrl.th->state++;
            }
            break;
        case 3:  // Set Time
            autom_cntrl.th->state = 10;
            if (autom_cntrl.set_time)
            {
                if (sema_reserve( SEMA_SERIAL2))
                {
                    SerialClock.printf("<C1TS:%04d;%02d;%02d;%02d;%02d>\r\n", 
                        main_ctrl.time.year,
                        main_ctrl.time.month,
                        main_ctrl.time.day,
                        main_ctrl.time.hour, 
                        main_ctrl.time.minute);
                    autom_cntrl.set_time = false;
                    sema_release( SEMA_SERIAL2);
                }              
            }
            break;
          case 10:  // Request Time
            autom_cntrl.th->state = 1;
            if (autom_cntrl.iter_cntr > autom_cntrl.next_get_time)
            {
                if (sema_reserve( SEMA_SERIAL2))
                {
                    SerialClock.printf("<C1TG:>\r\n");
                    autom_cntrl.next_get_time = autom_cntrl.iter_cntr + 60;
                    autom_cntrl.th->state = 20;
                    sema_release( SEMA_SERIAL2);
                }
            }
            break;
        case 20:
            autom_cntrl.th->state = 20;
            if (SerialClock.available())
            {
                str = SerialClock.readStringUntil('\n');
                if (str.length()> 0)
                {
                    if (autom_parse_time(&str))
                    {
                      autom_cntrl.th->wd_cntr = 0;
                      if ((main_ctrl.time.hour != autom_cntrl.last_time.hour) ||
                          (main_ctrl.time.minute > autom_cntrl.last_time.minute ))
                      {
                        autom_cntrl.last_time.hour = main_ctrl.time.hour;
                        autom_cntrl.last_time.minute = main_ctrl.time.minute;
                        autom_cntrl.th->state = 50;
                      }
                    }
                }
                supervisor_clr_cntr(SUPER_ERR_GET_TIME);
            }
            else
            {
                if (autom_cntrl.iter_cntr > autom_cntrl.next_get_time)  
                {
                    Serial.println("No get time response -> WD Reset");
                    autom_cntrl.th->wd_cntr = autom_cntrl.th->wd_limit; 
                    autom_cntrl.th->state = 200;  
                }
            }
            break;
        case 50:
            if (autom_cntrl.iter_cntr > autom_cntrl.next_random)
            {
                autom_randomize();
                autom_cntrl.next_random = autom_cntrl.iter_cntr + NEXT_RANDOM_ITER;  // seconds
                Serial.println("Randomizing");
            }
            autom_cntrl.relay_indx = 0;
            autom_cntrl.th->state++;
            break;
        case 51:
            bool changed;
            Serial.printf("Auto Relay Control: Month: %d Hour: %d\n", main_ctrl.time.month, main_ctrl.time.hour);
            do
            {

              
              changed = autom_on_off(autom_cntrl.relay_indx);
              if (changed) 
              {
                char rval = '0';
                if (autom_relay[autom_cntrl.relay_indx].autom_state == RELAY_AUTOM_STATE_ON) rval = '1';
                relay_send_one((va_relays_et)autom_cntrl.relay_indx, rval);
                autom_cntrl.th->state++;
              }
              autom_cntrl.relay_indx++;
            }
            while ( !changed && (autom_cntrl.relay_indx < VA_RELAY_NBR_OF));

            if ( autom_cntrl.relay_indx >= VA_RELAY_NBR_OF) autom_cntrl.th->state = 2;
            break;
        case 52:
             autom_cntrl.th->state--;
        case 100:
            //     for (uint8_t i = VA_RELAY_UNDEF; i < VA_RELAY_NBR_OF; i++)

            break;
        case 200:
            Serial.print("#");
            supervisor_inc_cntr(SUPER_ERR_GET_TIME);
            autom_cntrl.th->state = 2;  // TODO
            break;
    }

}


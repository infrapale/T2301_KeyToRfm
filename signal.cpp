#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "main.h"
#include "signal.h"
#include "autom.h"
#include "task.h"

#define RGB_PIX_PIN     22

typedef struct
{
  signal_event_et event;
  //signal_state_et state;
  //signal_state_et prev_state;
  signal_state_et state_request;
  task_st  *sm;
  relay_prog_et relay_prog;
  uint8_t one_pix_state;
  uint8_t seq_indx;
  uint8_t seq_cntr;
  uint32_t sm_millis;
} signal_st;


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel one_pix = Adafruit_NeoPixel(1, RGB_PIX_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

uint32_t signal_color[RGB_INDX_NBR_OF] = 
{
  RGB_BLACK,
  RGB_RED,
  RGB_GREEN,
  RGB_BLUE,
  RGB_WHITE,
  RGB_MAGENTA,
  RGB_CYAN,
  RGB_YELLOW,
} ;

signal_seq signal_pattern[SIGNAL_INDEX_NBR_OF] = 
{
  [SIGNAL_INDEX_START]      = {{RGB_INDX_YELLOW , 5}, {RGB_INDX_BLACK, 5},  {RGB_INDX_BLACK, 0}},
  [SIGNAL_INDEX_AT_HOME]    = {{RGB_INDX_GREEN  , 2}, {RGB_INDX_BLACK, 5},  {RGB_INDX_BLACK, 0}},
  [SIGNAL_INDEX_COUNTDOWN]  = {{RGB_INDX_YELLOW , 2}, {RGB_INDX_GREEN, 5},  {RGB_INDX_BLACK, 0}},
  [SIGNAL_INDEX_AWAY]       = {{RGB_INDX_RED , 4},    {RGB_INDX_YELLOW, 4}, {RGB_INDX_BLACK, 10}},
  [SIGNAL_INDEX_WARNING]    = {{RGB_INDX_RED , 1},    {RGB_INDX_BLUE, 1},   {RGB_INDX_BLACK, 2}},
  [SIGNAL_INDEX_ALARM]      = {{RGB_INDX_RED , 2},    {RGB_INDX_BLACK, 5},  {RGB_INDX_BLACK, 0}},
  [SIGNAL_INDEX_SENDING]    = {{RGB_INDX_CYAN , 2},   {RGB_INDX_BLACK, 2},  {RGB_INDX_BLACK, 0}},
};

char state_label[SIGNAL_INDEX_NBR_OF][10] =
{
// 0123456789  
  "Start    ",
  "Kotona   ",
  "Countdown",
  "Poissa   ",
  "Varoitus ",
  "Halytys  ",
  "Lahettaa "
};


signal_st signal;

void signal_initialize(void)
{
    uint32_t color_u32;
    
    signal.one_pix_state = 0;
    signal.event = SIGNAL_EVENT_UNDEFINED;
    signal.sm = task_get_task(TASK_SIGNAL_STATE);
    signal.sm->state = 0;
    signal.seq_indx = 0;
    signal.seq_cntr = 0;
    signal.relay_prog = RELAY_PROG_UNDEF;
    color_u32 =  RGB_MAGENTA; 
    one_pix.begin();
    one_pix.setBrightness(255);
    //one_pix.Color(255 ,0, 0);
    one_pix.setPixelColor(0, color_u32);
    one_pix.show(); // Initialize all pixels to 'off'
}


void signal_set_event(signal_event_et signal_event)
{
    signal.event = signal_event;
    Serial.printf("Set event %d\n\r",signal.event);
}


uint16_t signal_get_state(void)
{
   return ((uint16_t)signal.sm->state);
}

uint8_t signal_get_state_index(void)
{
   return ((uint8_t)signal.sm->state >> 4);
}

void signal_set_state(uint16_t new_state)
{
  signal.sm->state = new_state;
}

char *signal_get_state_label(void)
{
   uint8_t indx = (signal.sm->state >> 4) & 0x0F;
   return state_label[indx];
}


void signal_update(void)
{
    uint8_t ind_indx = signal.sm->state >> 4;
    if(++signal.seq_cntr > signal_pattern[ind_indx][signal.seq_indx].duration)
    {
        if(++signal.seq_indx >= NBR_COLOR_SEQ) signal.seq_indx = 0;
        if (signal_pattern[ind_indx][signal.seq_indx].duration == 0) signal.seq_indx = 0;
        signal.seq_cntr = 0;
        one_pix.setPixelColor(0, signal_color[signal_pattern[ind_indx][signal.seq_indx].color]);
        one_pix.show(); // Initialize all pixels to 'off'

    }

    // one_pix.setPixelColor(0, signal_color[signal.one_pix_state]);
    // one_pix.show(); // Initialize all pixels to 'off'
    // if(++signal.one_pix_state >= RGB_INDX_NBR_OF) signal.one_pix_state = 0;      


}


void signal_state_machine(void)
{
    static uint8_t prev_state = 255;
    static uint8_t prev_event = 255;
    if(millis() > signal.sm_millis)
    {
      //Serial.print('*');
      if (signal.event ==  SIGNAL_EVENT_UNDEFINED)
      {
          //Serial.print('-');
          signal.event = SIGNAL_EVENT_TIMEOUT;
          signal.sm_millis = 0xFFFFFFFF;
      }
    }   
    if ((signal.event != prev_event) || (signal.sm->state != prev_state))
    {
        Serial.printf("State %d, Event = %d\n\r", signal.sm->state, signal.event);
        //Serial.printf("millis %d, sm_millis %d\n\r", millis(), signal.sm_millis);
    }
    switch(signal.sm->state)
    {
      case SIGNAL_STATE_START:  // Starting ...
        signal.sm_millis = millis() + 1000;
        autom_set_program(RELAY_PROG_UNDEF);
        signal.sm->state = SIGNAL_STATE_START + 1;
        break;

      case SIGNAL_STATE_START + 1:  // Start state
        // TODO read from edog
        signal.sm->state = SIGNAL_STATE_AT_HOME;
        autom_set_program(RELAY_PROG_UNDEF);
        signal.sm->state = SIGNAL_STATE_AT_HOME;
        break;

      case SIGNAL_STATE_AT_HOME:   // At home state
        switch(signal.event)
        {
          case SIGNAL_EVENT_SENDING:
            signal.sm_millis = millis() + 5000;
            signal.sm->state = SIGNAL_STATE_SENDING;
            break;
          case SIGNAL_EVENT_LEAVE:
            signal.sm_millis = millis() + 30000;
            signal.sm->state = SIGNAL_STATE_COUNTDOWN;
            break;          
          case SIGNAL_EVENT_ALERT:
            signal.relay_prog = RELAY_PROG_WARNING ;
            signal.sm->state = SIGNAL_STATE_WARNING;
            break;
        }        
        break;

      case SIGNAL_STATE_COUNTDOWN:  // Countdown
        if (signal.event == SIGNAL_EVENT_TIMEOUT) 
        {
          signal.sm->state = SIGNAL_STATE_AWAY;
        }         
        break;
      case SIGNAL_STATE_AWAY:
        switch(signal.event)
        {
          case SIGNAL_EVENT_LOGIN:
            signal.sm_millis = millis() + 30000;
            signal.sm->state = SIGNAL_STATE_AT_HOME;
            break;          
          case SIGNAL_EVENT_ALERT:
            signal.sm->state = SIGNAL_STATE_ALARM;
            signal.relay_prog = RELAY_PROG_ALARM;
            break;
        }
      case SIGNAL_STATE_ALARM:
        switch(signal.event)
        {
          case SIGNAL_EVENT_CONFIRM:
            signal.sm->state = SIGNAL_STATE_AWAY;
            signal.relay_prog = RELAY_PROG_AWAY;
            break;          
        }
        break;
      case SIGNAL_STATE_WARNING:
        switch(signal.event)
        {
          case SIGNAL_EVENT_CONFIRM:
          case SIGNAL_EVENT_TIMEOUT:
            signal.sm->state = SIGNAL_STATE_AWAY;
            signal.relay_prog = RELAY_PROG_AWAY;
            break;          
        }
        break;

      case SIGNAL_STATE_SENDING:
        if (signal.event == SIGNAL_EVENT_TIMEOUT) 
        {
          signal.sm->state = SIGNAL_STATE_AT_HOME;
        }         
        break;
    }
    signal.event = SIGNAL_EVENT_UNDEFINED;
    prev_state = signal.sm->state;
    prev_event = signal.event;

}
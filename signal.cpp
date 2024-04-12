#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include "main.h"
#include "signal.h"

#define RGB_PIX_PIN     22

typedef struct
{
  signal_state_et state;
  signal_state_et prev_state;

  uint8_t one_pix_state;
  uint8_t seq_indx;
  uint8_t seq_cntr;
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

signal_seq signal_pattern[SIGNAL_NBR_OF] = 
{
  [SIGNAL_START]      = {{RGB_INDX_YELLOW , 5}, {RGB_INDX_BLACK, 5},  {RGB_INDX_BLACK, 0}},
  [SIGNAL_AT_HOME]    = {{RGB_INDX_GREEN  , 2}, {RGB_INDX_BLACK, 5},  {RGB_INDX_BLACK, 0}},
  [SIGNAL_COUNTDOWN]  = {{RGB_INDX_YELLOW , 2}, {RGB_INDX_GREEN, 5},  {RGB_INDX_BLACK, 0}},
  [SIGNAL_ALARM]      = {{RGB_INDX_RED , 2}, {RGB_INDX_BLACK, 5},  {RGB_INDX_BLACK, 0}},
  [SIGNAL_SENDING]    = {{RGB_INDX_CYAN , 2}, {RGB_INDX_BLACK, 2},  {RGB_INDX_BLACK, 0}},
};

signal_st signal;

void signal_initialize(void)
{
    uint32_t color_u32;
    
    signal.one_pix_state = 0;
    signal.state = SIGNAL_START;
    signal.prev_state = SIGNAL_START;

    signal.seq_indx = 0;
    signal.seq_cntr = 0;
    color_u32 =  RGB_MAGENTA; 
    one_pix.begin();
    one_pix.setBrightness(255);
    //one_pix.Color(255 ,0, 0);
    one_pix.setPixelColor(0, color_u32);
    one_pix.show(); // Initialize all pixels to 'off'
}

void signal_set_state(signal_state_et new_state)
{
    signal.prev_state = signal.state;
    signal.state = new_state;
}

void signal_return_state(void)
{
    signal.state = signal.prev_state;
}


void signal_update(void)
{

    if(++signal.seq_cntr > signal_pattern[signal.state][signal.seq_indx].duration)
    {
        if(++signal.seq_indx >= NBR_COLOR_SEQ) signal.seq_indx = 0;
        if (signal_pattern[signal.state][signal.seq_indx].duration == 0) signal.seq_indx = 0;
        signal.seq_cntr = 0;
        one_pix.setPixelColor(0, signal_color[signal_pattern[signal.state][signal.seq_indx].color]);
        one_pix.show(); // Initialize all pixels to 'off'

    }

    // one_pix.setPixelColor(0, signal_color[signal.one_pix_state]);
    // one_pix.show(); // Initialize all pixels to 'off'
    // if(++signal.one_pix_state >= RGB_INDX_NBR_OF) signal.one_pix_state = 0;      


}
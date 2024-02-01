#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "main.h"
#define PIN               22
#define NBR_OF_PIX        24
#define NBR_OF_HOURS      24
#define MINUTES_PER_HOUR  60
#define FULL_DAY_MINUTES  (NBR_OF_HOURS * MINUTES_PER_HOUR)
#define HALF_DAY_MINUTES  (12*MINUTES_PER_HOUR)
#define ZERO_PIX          6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

uint16_t hour_minutes[NBR_OF_HOURS*2];
extern main_ctrl_st main_ctrl;

void clock24_initialize(void)
{
   for (uint16_t h = 0; h < NBR_OF_HOURS + 1; h++ )
  {
      hour_minutes[h] = h * MINUTES_PER_HOUR;
  }

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
}

uint16_t clock24_minute_distance(uint8_t hour, uint8_t minute, uint16_t scale_hour)
{
    int16_t dist;
    uint16_t minutes_now;
    uint16_t minutes_scale;
    uint8_t  h2 = hour;

    minutes_now = hour_minutes[hour] + minute;
    minutes_scale = hour_minutes[scale_hour];
    dist = abs(minutes_now-minutes_scale);
    if (minutes_scale >  minutes_now )
    {
        if((minutes_now < HALF_DAY_MINUTES) && (minutes_scale > HALF_DAY_MINUTES))
            dist = minutes_now + (FULL_DAY_MINUTES - minutes_scale);
    }
    else 
    {
        if((minutes_now > HALF_DAY_MINUTES) && (minutes_scale < HALF_DAY_MINUTES))
            dist = minutes_scale + (FULL_DAY_MINUTES - minutes_now);    
    }
    return dist;
}

void clock_show(uint8_t hour, uint8_t minute)
{
    uint8_t h = hour;
    uint8_t m = minute;
    int16_t d = 0;
    uint32_t color_u32;
    uint8_t  intens;


    //Serial.printf("%02d:%02d\n",h, m);
    for (int hx = 0; hx < NBR_OF_HOURS; hx++ )
    {
        d = clock24_minute_distance(h,m,hx);
        Serial.printf("%02d:%02d ",h, m);
        Serial.printf("hx=%02d d=%06d %02d:%02d\n", hx, d, d/60, d % 60);
        color_u32 = strip.Color(0, 0, 0);
        intens = 0;
        // if ( d < 30) color_u32 = strip.Color(0,255, 0);
        // else if ( d < 60) color_u32 = strip.Color( 0, 40,0);
        // else if ( d < 120) color_u32 = strip.Color( 0, 10, 0);
        if ( d < 30) intens = 255;
        else if ( d < 60) intens = 40;
        else if ( d < 120) intens = 10;
        switch (main_ctrl.status)
        {
          case STATUS_AT_HOME:
            color_u32 = strip.Color( 0, intens, 0);
            break;
          case STATUS_AWAY:
            color_u32 = strip.Color( intens, 0, 0);
            break;
        }
        strip.setPixelColor(hx, color_u32);
    }
    strip.show();
}
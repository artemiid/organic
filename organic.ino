// Artimiid art boat 
#include <FastLED.h>
#include "timer.h"

#define LEDS_PIN        10
#define NUM_LEDS        120
#define BRIGHTNESS      255

#define PATTERN_TIME    60000
#define CROSSFADE_TIME  2500

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
CRGB leds_1[NUM_LEDS];
CRGB leds_2[NUM_LEDS];

// 1 - 10
const uint8_t address = 9;

const TProgmemRGBPalette16 Artemiid_Colors_p FL_PROGMEM =
{
    CRGB::Black,
    CRGB::Indigo,
    CRGB::Aquamarine,
    CRGB::DarkSlateBlue,    
    
    CRGB::Amethyst,
    CRGB::SeaGreen,
    CRGB::Teal,
    CRGB::Black,

    CRGB::Black,
    CRGB::BlueViolet,
    CRGB::DarkMagenta,
    CRGB::DarkOrchid,
    
    CRGB::Indigo,
    CRGB::LightSeaGreen,
    CRGB::LightSkyBlue,
    CRGB::Black,
};


//--------------------------------------------------------------------------------------------------
// PATTERNS

CRGBPalette16 current_palette = Artemiid_Colors_p;

void rainbow(CRGB *leds) {
   static uint8_t hue = 0;
   fill_rainbow(leds, NUM_LEDS, hue, 7);
   hue++;
}

uint16_t distance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint16_t dx = x2 - x1;
    uint16_t dy = y2 - y1;
    return sqrt16((dx * dx) + (dy * dy));
}


void plasma(CRGB *leds) {
  // loosely based on http://www.mennovanslooten.nl/blog/post/72
  
   uint8_t offset1 = beat8(3);
   uint8_t offset2 = beat8(7); 

   uint8_t y=address * 12;
   
   for (uint8_t x=0; x<NUM_LEDS; x++) {
      uint8_t i1 = (x * -4) + offset1;
      uint8_t i2 = distance(x * 2, y * 2, sin8(-offset2), cos8(-offset2));
      
      nblend(leds[x], ColorFromPalette(current_palette, avg8(i1, i2), 255, LINEARBLEND), 128);
   }
   
   fadeLightBy(leds, NUM_LEDS, 10);
}

void plasma_glitter(CRGB *leds) {
    plasma(leds);
    leds[ random8(NUM_LEDS) ] += CRGB::White;
}

void big_confetti(CRGB * leds) {
    fadeToBlackBy( leds, NUM_LEDS, 4);
    if (random8() < 120) {
        int pos = random8(3, NUM_LEDS - 3);
        //int hue = random8(256);
        CRGB c = ColorFromPalette(current_palette, random8(), 255, LINEARBLEND);
        leds[pos-1] += blend(c, CRGB::Black, random8(100));
        leds[pos  ] += blend(c, CRGB::Black, random8(100, 200));
        leds[pos+1] += blend(c, CRGB::Black, random8(100));
    }
}

/*
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void draw_worm(CRGB * leds, float head, float tail, uint8_t color) {
    if (tail > head) {
        head += tail;
        tail = head - tail;
        head -= tail;
    }
    
    float brightness = 200.0f;
    brightness -= mapfloat(head - tail, 8.0f, 30.0f, 0.0f, 180.0f);
  
    leds[(int)head + 1] += ColorFromPalette(current_palette, color, fmod(head, 1) * brightness);
    for (int i = (int)tail + 1; i <= (int)head; i++) {
        leds[i] += ColorFromPalette(current_palette, color, brightness);
    }
    leds[(int)tail] += ColorFromPalette(current_palette, color, (1.0f - fmod(tail, 1)) * brightness);
}

struct Worm {

    bool growing;
    float worm_head;
    float worm_tail;
    float dir;
    float step_size;
    uint8_t steps_per_second;
  
    Worm() {
      this->growing = true;
      this->steps_per_second = random8(20, 50);
      this->step_size = random8(128, 255) / 255.0f * 12.0f;
      this->dir = random8() > 127 ? 1.0f : -1.0f;

      uint8_t worm_size = random8(8, 25);
      this->worm_head = random8(worm_size + 1, NUM_LEDS - worm_size - 1);
      this->worm_tail = worm_head + (dir * worm_size);
    }

    void render(CRGB * leds) {
      float head, tail;
  
      // the worm grows in the first half and shrinks in the second
      uint8_t phase = beat8(steps_per_second);
  
      if ((dir > 0.0f && worm_head + step_size >= NUM_LEDS) || 
          (dir < 0.0f && worm_head - step_size <= 0.0f)) {
            dir *= -1.0f;
            worm_head += worm_tail;
            worm_tail = worm_head - worm_tail;
            worm_head -= worm_tail;
      }
  
  
      float delta = cubicwave8(phase % 128) / 255.0f * step_size * dir;
  
  
      head = worm_head;
      tail = worm_tail;
  
      if (phase < 128) {
          if (!growing) {
              growing = true;
              worm_tail += (step_size * dir);
              tail = worm_tail;
          }
          head += delta;
      } else {
          if (growing) {
              growing = false;
              worm_head += (step_size * dir);
              head = worm_head;
          }
          tail += delta;
      }
      draw_worm(leds, head, tail, beat8(1) + steps_per_second);        
    }
};

//Worm(float head, float tail, uint8_t steps_per_second, float step_size, uint8_t color) {
void worms(CRGB * leds) {
  
    static Worm w1;
    static Worm w2;
 
   memset(leds, 0, sizeof(CRGB) * NUM_LEDS);

   //current_palette = Worm_Colors_p;
   
   w1.render(leds);
   w2.render(leds);
}
*/
//--------------------------------------------------------------------------------------------------
// BOILERPLATE

typedef void (*SimplePatternList[])(CRGB *);
SimplePatternList patterns = { 
  big_confetti,
  plasma,
  big_confetti,
  plasma,
  big_confetti,
  plasma_glitter,
};

uint8_t current_pattern = 0;

MillisTimer fade_timer;
MillisTimer change_pattern_timer;

void setup() {
  
  // led strip
  FastLED.addLeds<NEOPIXEL, LEDS_PIN>(leds_1, NUM_LEDS);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  random16_set_seed(address * 77);

  change_pattern_timer.start(PATTERN_TIME, true);
  current_palette = Artemiid_Colors_p;
}
  
void loop()
{  
  // are we done changing patterns?
  if (fade_timer.fired()) {
      current_pattern = get_next_pattern(current_pattern);
  }
  
  // Call the current pattern function
  patterns[current_pattern](leds_1);

  // time to change pattern?
  if (change_pattern_timer.fired()) {
      fade_timer.start(CROSSFADE_TIME, false);
  }

  // if we are in the middle of changing patterns then render the next one and blend it
  if (fade_timer.running()) {
      patterns[get_next_pattern(current_pattern)](leds_2);
      nblend(leds_1, leds_2, NUM_LEDS, fade_timer.perc8());
  }

  FastLED.show();
  FastLED.delay(1000/120);
}

uint8_t  get_next_pattern(uint8_t current) {
    return (current + 1) % ARRAY_SIZE(patterns);
}


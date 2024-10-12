#include "LED.h"
#include <FastLED.h>

THERMAL_PRESENCE_NAMESPACE_BEGIN

CRGB leds[1];

void LED::begin(uint8_t brightness) {
    FastLED.addLeds<NEOPIXEL, 35>(leds, 1);
    FastLED.setBrightness(brightness);
}

void LED::set(CRGB color) {
    leds[0] = color;
    FastLED.show();
}

THERMAL_PRESENCE_NAMESPACE_END
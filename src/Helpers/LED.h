#ifndef __LED_H
#define __LED_H

#include <crgb.h>
#include "namespace.h"

THERMAL_PRESENCE_NAMESPACE_BEGIN

class LED {
	public:
	static void begin(uint8_t brightness = 20);
	static void set(CRGB color);
};

THERMAL_PRESENCE_NAMESPACE_END

#endif
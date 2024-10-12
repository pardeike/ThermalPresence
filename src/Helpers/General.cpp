#include "General.h"
#include "LED.h"
#include "Thermal.h"

#include <M5Unified.h>
#include <Wire.h>

THERMAL_PRESENCE_NAMESPACE_BEGIN

void General::init() {
    M5.begin();

    LED::begin(20);

    Serial.begin(115200);
    while (!Serial);

    Thermal::init();
}

THERMAL_PRESENCE_NAMESPACE_END
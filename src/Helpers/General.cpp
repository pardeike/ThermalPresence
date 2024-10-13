#include "General.h"
#include "LED.h"
#include "Network.h"
#include "Thermal.h"

#include <M5Unified.h>
#include <Wire.h>

THERMAL_PRESENCE_NAMESPACE_BEGIN

void General::init(char *deviceName, char *ssid, char *password) {
    M5.begin();

    LED::begin(20);

    Serial.begin(115200);
    while (!Serial);

    Network::init(deviceName, ssid, password);
    Thermal::init();
}

THERMAL_PRESENCE_NAMESPACE_END
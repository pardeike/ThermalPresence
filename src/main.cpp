#include "all_includes.h"

float *pixels = Thermal::createBuffer();

void setup() {
	General::init();
	LED::set(CRGB::Green);
}

void loop() {
	M5.delay(1);
	M5.update();
	if (M5.BtnA.wasPressed()) {
		LED::set(CRGB::Red);
		auto startTime = micros();
		Thermal::readThermal(pixels);
		unsigned long ms = micros() - startTime;
		LED::set(CRGB::Black);
		Thermal::debugPixels(pixels);
		Serial.println("scan took " + String(ms) + " micro seconds");
	}
}
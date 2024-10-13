#include "Network.h"

#include <M5Unified.h>
#include <HTTPClient.h>
#include <WiFi.h>

THERMAL_PRESENCE_NAMESPACE_BEGIN

HTTPClient http;

void Network::init(char *deviceName, char *ssid, char *password) {
	WiFi.begin();
	WiFi.setHostname(deviceName);
	WiFi.begin(ssid, password);
	while (WiFi.status()!= WL_CONNECTED);
}

int Network::send(String url, const char *contentType, uint8_t *data, size_t length) {
	http.begin(url);
	http.addHeader("Content-Type", contentType);
	http.addHeader("Content-Length", String(length));
	int httpCode = http.POST(data, length);
	http.end();
	return httpCode;
}

THERMAL_PRESENCE_NAMESPACE_END
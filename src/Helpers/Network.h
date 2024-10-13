#ifndef __NETWORK_H
#define __NETWORK_H

#include "namespace.h"
#include <Arduino.h>

THERMAL_PRESENCE_NAMESPACE_BEGIN

class Network {
	public:
	static void init(char *deviceName, char *ssid, char *password);
	static int send(String url, const char *contentType, uint8_t *data, size_t length);
};

THERMAL_PRESENCE_NAMESPACE_END

#endif
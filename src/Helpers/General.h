#ifndef __GENERAL_H
#define __GENERAL_H

#include "namespace.h"

THERMAL_PRESENCE_NAMESPACE_BEGIN

class General {
	public:
	static void init(char *deviceName, char *ssid, char *password);
};

THERMAL_PRESENCE_NAMESPACE_END

#endif
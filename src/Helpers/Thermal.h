#ifndef __THERMAL_H
#define __THERMAL_H

#include "namespace.h"

THERMAL_PRESENCE_NAMESPACE_BEGIN

extern int THERMAL_COLS;
extern int THERMAL_ROWS;

class Thermal {
	public:
	static int init();
	static float * createBuffer();
	static void readImage(float *pixels);
	static int readThermal(float *pixels);
	static void debugPixels(float *pixels);
};

THERMAL_PRESENCE_NAMESPACE_END

#endif
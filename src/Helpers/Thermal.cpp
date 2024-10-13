#include "Thermal.h"
#include <Wire.h>
#include <M5Unified.h>
#include "../Mlx90640/MLX90640_API.h"
#include "../Mlx90640/MLX90640_I2C_Driver.h"

THERMAL_PRESENCE_NAMESPACE_BEGIN

int THERMAL_COLS = 32;
int THERMAL_ROWS = 24;

const byte MLX90640_address = 0x33;
paramsMLX90640 mlx90640;
uint16_t eeMLX90640[832];
char chars[] = "`.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@";

#define TA_SHIFT 8

// useful info:
// https://cdn.hackaday.io/files/1614996909573216/MLX90640%20driver.pdf

int Thermal::init() {
  Wire.begin();
  Wire.setClock(1000000UL);

  int sda = M5.Ex_I2C.getSDA();
  int scl = M5.Ex_I2C.getSCL();
  M5.Ex_I2C.begin(MLX90640_address, sda, scl);

  while (true) {
    Wire.beginTransmission(MLX90640_address);
    if (Wire.endTransmission() == 0)
      break;
  }

	int status;
	status = MLX90640_DumpEE(MLX90640_address, eeMLX90640);
	if (status != 0) {
		Serial.print("MLX90640_DumpEE returned ");
		Serial.println(status);
		return -1;
	}

	status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
	if (status != 0) {
		Serial.print("MLX90640_ExtractParameters returned ");
		Serial.println(status);
		return -2;
	}

	MLX90640_SetRefreshRate(MLX90640_address, 0x04);
  MLX90640_SetResolution(MLX90640_address, 0x00);

  MLX90640_SetChessMode(MLX90640_address);
  //MLX90640_SetInterleavedMode(MLX90640_address);

  return 0;
}

float * Thermal::createBuffer() {
  return new float[THERMAL_COLS * THERMAL_ROWS];
}

void Thermal::readImage(float *pixels) {
  uint16_t mlx90640Frame[834];
  MLX90640_GetImage(mlx90640Frame, &mlx90640, pixels);
}

int Thermal::readThermal(float *pixels) {
  uint16_t mlx90640Frame[834];
  int result = MLX90640_GetFrameData(MLX90640_address, mlx90640Frame);
  if (result < 0) {
    Serial.print("MLX90640_GetFrameData = ");
    Serial.println(result);
    return -1;
  }

  float vdd = MLX90640_GetVdd(mlx90640Frame, &mlx90640);
  float Ta = MLX90640_GetTa(mlx90640Frame, &mlx90640);
  float tr = Ta - TA_SHIFT;
  float emissivity = 0.95;
  MLX90640_CalculateTo(mlx90640Frame, &mlx90640, emissivity, tr, pixels);
  return MLX90640_GetSubPageNumber(mlx90640Frame);;
}

void Thermal::debugPixels(float *pixels) {
  float min = 1000;
  float max = -1000;
  for(int i = 0; i < THERMAL_COLS * THERMAL_ROWS; i++) {
    float f = pixels[i];
    if (f < min) min = f;
    if (f > max) max = f;
  }
  for(int y = 0; y < THERMAL_ROWS; y++) {
    for(int x = 0; x < THERMAL_COLS; x++) {
      if (x > 0) Serial.print(" ");
      int idx = (int)((pixels[y * THERMAL_COLS + (THERMAL_COLS - 1 - x)] - min) /  (max - min) * strlen(chars));
      Serial.print(chars[idx]);
    }
    Serial.println();
  }
}

THERMAL_PRESENCE_NAMESPACE_END
#include "all_includes.h"

// TODO
// https://forum.arduino.cc/t/temperature-array-best-way-to-to-recognize-humans/597915/4
// https://github.com/Makerfabs/ESP32-S2-MLX90640-Touch-Screen-Thermal-Camera
// https://github.com/Makerfabs/ESP32-S2-MLX90640-Touch-Screen-Thermal-Camera/tree/main/firmware/Thermal_Camera_S3

auto pixels = Thermal::createBuffer();
auto totalPixelCount = THERMAL_COLS * THERMAL_ROWS;
auto averagedPixels = (float *)calloc(totalPixelCount, sizeof(float));
auto bytes = (uint8_t *)calloc(totalPixelCount, sizeof(uint8_t));
int histogram[256];

String url = "http://AndreassAir.local/index.html";

void setup() {
	General::init("ThermalPresence", "AP iPhone", "12345678");
	LED::set(CRGB::Green);
	M5.delay(500);
}

template <typename T>
std::tuple<T, T> findMinMax(T* array, int size) {
	T min = std::numeric_limits<T>::max();
	T max = std::numeric_limits<T>::min();
	for (int i = 0; i < size; ++i) {
		if (array[i] < min) min = array[i];
		if (array[i] > max) max = array[i];
	}
	return std::make_tuple(min, max);
}

void sendValues(float* values) {
	auto [min, max] = findMinMax(values, totalPixelCount);

	int i = 0;
	for(int y = 0; y < THERMAL_ROWS; y++)
		for(int x = 0; x < THERMAL_COLS; x++)
			bytes[i++] = (uint8_t)((values[y * THERMAL_COLS + x] - min) * 255 / (max - min));

	for(int i = 0; i < totalPixelCount; i++)
		histogram[bytes[i]]++;
	auto [minHist, maxHist] = findMinMax(histogram, 256);

	int cutoffLevel = minHist + (maxHist - minHist) * 4 / 5;
	for(int i = 0; i < 256; i++)
		if (histogram[i] < cutoffLevel) {
			for(int j = 0; j < totalPixelCount; j++)
				if (bytes[j] == i)
					bytes[j] = i < 128 ? 0 : 255;
		}

	Network::send(url, "application/octet-stream", bytes, totalPixelCount);
}

void filter_frame(float *in, float *out) {
	for (int i = 0; i < THERMAL_ROWS; i++)
		for (int j = 0; j < THERMAL_COLS; j++)
			out[THERMAL_COLS * i + 31 - j] = (out[THERMAL_COLS * i + 31 - j] + in[THERMAL_COLS * i + j]) / 2;
}

void loop() {

	LED::set(CRGB::Red);
	Thermal::readThermal(pixels);

	LED::set(CRGB::Blue);
	filter_frame(pixels, averagedPixels);
	sendValues(averagedPixels);

	LED::set(CRGB::Black);
	M5.delay(100);
}
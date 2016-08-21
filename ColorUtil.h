#ifndef HEADER_COLOR_UTIL
#define HEADER_COLOR_UTIL

#include <NeoPixelBus.h>

RgbwColor hsi2rgbw(unsigned char Hbyte, float S, float I);
String rgbw2html(RgbwColor& col);
RgbwColor html2rgbw(String html);
RgbwColor applyBrightness(RgbwColor in, byte brightness);
RgbwColor scaleRGB(RgbwColor in, byte scale);

#endif

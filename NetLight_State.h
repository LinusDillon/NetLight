#ifndef HEADER_NETLIGHT_STATE
#define HEADER_NETLIGHT_STATE

#include <NeoPixelBus.h>
#include <ArduinoJson.h>

#define LIGHTMODE_FIXED 1
#define LIGHTMODE_CYCLE_RAINBOW 2
#define LIGHTMODE_CYCLE_TWO_COLOR 3
#define LIGHTMODE_CYCLE_RANDOM 4
#define LIGHTMODE_WAVES 5
#define LIGHTMODE_SPARKLES 6

class NetLightState 
{
  public:
    bool lightEnabled = false;
    unsigned char lightLevel = 255;
    unsigned char lightMode = LIGHTMODE_FIXED;
    RgbwColor color1;
    RgbwColor color2;
    int time1_min;
    int time1_max;
    int time2_min;
    int time2_max;

    String toString();
    bool fromString(String csv);
};

extern NetLightState state;

#endif

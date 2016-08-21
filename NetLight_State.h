#ifndef HEADER_NETLIGHT_STATE
#define HEADER_NETLIGHT_STATE

#include <NeoPixelBus.h>
#include <ArduinoJson.h>

#define ANIM_CHANELS 16

#define LIGHTMODE_NONE 0
#define LIGHTMODE_FIXED 1
#define LIGHTMODE_CYCLE_RAINBOW 2
#define LIGHTMODE_CYCLE_TWO_COLOR 3
#define LIGHTMODE_CYCLE_RANDOM 4
#define LIGHTMODE_WAVES 5
#define LIGHTMODE_SPARKLES 6

class NetLightStateAnim
{
  public:
    unsigned char lightMode = LIGHTMODE_NONE;
    RgbwColor color1 = RgbwColor(255, 0, 0);
    RgbwColor color2 = RgbwColor(0, 255, 0);
    int time1_min = 5000;
    int time1_max = 5000;
    int time2_min = 10000;
    int time2_max = 10000;

    String toString();
    bool fromString(String csv, int* pos);
};

class NetLightState 
{
  public:
    bool lightEnabled = false;
    unsigned char lightLevel = 255;
    NetLightStateAnim animationStates[ANIM_CHANELS];

    String toString();
    bool fromString(String csv);
    String allAnimToString();
    String animToString(int index);
    int animFromString(String csv);
};


extern NetLightState state;


#endif

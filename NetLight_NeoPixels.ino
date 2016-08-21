RgbwColor targetColour(0, 0, 0, 0);

void StartStateAnimation(int index)
{
  switch (state.animationStates[index].lightMode)
  {
    case LIGHTMODE_FIXED:
      espDebug.println("LIGHTMODE_FIXED");  
      animations.StartAnimation(index, 10000, LightModeFixed);
      break;
    case LIGHTMODE_CYCLE_RAINBOW:
      espDebug.println("LIGHTMODE_CYCLE_RAINBOW");  
      animations.StartAnimation(index, random(state.animationStates[index].time1_min, state.animationStates[index].time1_max), LightModeRainbow);
      break;
    case LIGHTMODE_CYCLE_TWO_COLOR:
      espDebug.println("LIGHTMODE_CYCLE_TWO_COLOR");  
      animations.StartAnimation(index, random(state.animationStates[index].time1_min, state.animationStates[index].time1_max), LightModeCycleTwoColor);
      break;
    case LIGHTMODE_SPARKLES:
      espDebug.println("LIGHTMODE_SPARKLES");  
      animations.StartAnimation(index, random(state.animationStates[index].time1_min, state.animationStates[index].time1_max), LightModeSparkles);
      break;
  }
}

// Fixed colour
void LightModeFixed(const AnimationParam& param)
{
  targetColour = state.animationStates[param.index].color1;
  strip.ClearTo(targetColour);
}

// Rainbow - cycle through rainbow
void LightModeRainbow(const AnimationParam& param)
{
  targetColour = Wheel(param.progress * 255);
  strip.ClearTo(targetColour);
}

// Cycle between two colors - cycle through rainbow
void LightModeCycleTwoColor(const AnimationParam& param)
{
  float progress = param.progress * 2;
  if (progress > 1)
  {
    progress = 2 - progress;
  }
  targetColour = RgbwColor::LinearBlend(
        state.animationStates[param.index].color1,
        state.animationStates[param.index].color2,
        progress);
  strip.ClearTo(targetColour);  
}

// Select some random pixels (10%), and set them to the given colour
void LightModeSparkles(const AnimationParam& param)
{
  int numSparkles = PixelCount / 10;
  for (int i = 0; i < numSparkles; i ++) {    
    strip.SetPixelColor(random(PixelCount), state.animationStates[param.index].color1);
  }
}

RgbwColor Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85)
    {
        return RgbwColor(255 - WheelPos * 3, 0, WheelPos * 3, 0);
    }
    else if(WheelPos < 170)
    {
        WheelPos -= 85;
        return RgbwColor(0, WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else
    {
        WheelPos -= 170;
        return RgbwColor(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
    }
}

void ApplyBrightnessToStrip()
{
    int numPixelBytes = strip.PixelsSize();
    uint8_t* pixels = strip.Pixels();
    for (int i = 0; i < numPixelBytes; i ++)
    {
      pixels[i] = (uint16_t)pixels[i] * state.lightLevel / 256;
    }
}

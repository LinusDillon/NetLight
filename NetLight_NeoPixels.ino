RgbwColor targetColour(0, 0, 0, 0);

void StartStateAnimation()
{
  espDebug.println("StartStateAnimation");  
  switch (state.lightMode)
  {
    case LIGHTMODE_FIXED:
      espDebug.println("LIGHTMODE_FIXED");  
      animations.StartAnimation(0, 10000, LightModeFixed);
      break;
    case LIGHTMODE_CYCLE_RAINBOW:
      espDebug.println("LIGHTMODE_CYCLE_RAINBOW");  
      animations.StartAnimation(0, random(state.time1_min, state.time1_max), LightModeRainbow);
      break;
  }
}

// Fixed colour
void LightModeFixed(const AnimationParam& param)
{
  targetColour = state.color1;
  targetColour.Darken(255 - state.lightLevel);
  strip.ClearTo(targetColour);
}

// Rainbow - cycle through rainbow
void LightModeRainbow(const AnimationParam& param)
{
  targetColour = Wheel(param.progress * 255);
  targetColour.Darken(255 - state.lightLevel);
  strip.ClearTo(targetColour);
}

//// simple blend function
//void BlendAnimUpdate(const AnimationParam& param)
//{
//    // this gets called for each animation on every time step
//    // progress will start at 0.0 and end at 1.0
//    // we use the blend function on the RgbColor to mix
//    // color based on the progress given to us in the animation
//    RgbwColor updatedColor = RgbwColor::LinearBlend(
//        animationState[param.index].StartingColor,
//        animationState[param.index].EndingColor,
//        param.progress);
//
//    // apply the color to the strip
//    for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
//    {
//        strip.SetPixelColor(pixel, updatedColor);
//    }
//}
//
//void FlowBlendAnimUpdate(const AnimationParam& param)
//{
//    RgbwColor updatedColor = RgbwColor::LinearBlend(
//        animationState[param.index].StartingColor,
//        animationState[param.index].EndingColor,
//        param.progress);
//
//    // Cycle all the pixels down one
//    for (uint16_t pixel = 1; pixel < PixelCount; pixel++)
//    {
//        strip.SetPixelColor(pixel, strip.GetPixelColor(pixel - 1));
//    }
//
//    // Add the next blended colour at the start of the strip
//    strip.SetPixelColor(0, updatedColor);
//}

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

RgbwColor ScaleRGB(RgbwColor in, byte scale)
{
  return RgbwColor((uint16_t)in.R * scale / 255, (uint16_t)in.G * scale / 255, (uint16_t)in.B * scale / 255, 255 - scale);
}


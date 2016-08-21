#include "ColorUtil.h"
#include <math.h>

RgbwColor hsi2rgbw(unsigned char Hbyte, float S, float I) 
{
  int r, g, b, w;
  float cos_h, cos_1047_h, H;
  H = (float)Hbyte / (float)256 * 360; // cycle H around to 0-360 degrees
  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
  
  if(H < 2.09439) {
    cos_h = cos(H);
    cos_1047_h = cos(1.047196667-H);
    r = S*255*I/3*(1+cos_h/cos_1047_h);
    g = S*255*I/3*(1+(1-cos_h/cos_1047_h));
    b = 0;
    w = 255*(1-S)*I;
  } else if(H < 4.188787) {
    H = H - 2.09439;
    cos_h = cos(H);
    cos_1047_h = cos(1.047196667-H);
    g = S*255*I/3*(1+cos_h/cos_1047_h);
    b = S*255*I/3*(1+(1-cos_h/cos_1047_h));
    r = 0;
    w = 255*(1-S)*I;
  } else {
    H = H - 4.188787;
    cos_h = cos(H);
    cos_1047_h = cos(1.047196667-H);
    b = S*255*I/3*(1+cos_h/cos_1047_h);
    r = S*255*I/3*(1+(1-cos_h/cos_1047_h));
    g = 0;
    w = 255*(1-S)*I;
  }
  return RgbwColor(r, g, b, w);
}

//Converts the upper nibble of a binary value to a hexadecimal ASCII byte.
//For example, btohexa_high(0xAE) will return 'A'.
unsigned char btohexa_high(unsigned char b)
{
    b >>= 4;
    return (b>0x9u) ? b+'A'-10:b+'0';
}

//Converts the lower nibble of a binary value to a hexadecimal ASCII byte.
//  For example, btohexa_low(0xAE) will return 'E'.
unsigned char btohexa_low(unsigned char b)
{
    b &= 0x0F;
    return (b>9u) ? b+'A'-10:b+'0';
}

String rgbw2html(RgbwColor& col)
{
  char buf[10];
  buf[0] = '#';
  buf[1] = btohexa_high(col.R);
  buf[2] = btohexa_low(col.R);
  buf[3] = btohexa_high(col.G);
  buf[4] = btohexa_low(col.G);
  buf[5] = btohexa_high(col.B);
  buf[6] = btohexa_low(col.B);
  buf[7] = btohexa_high(col.W);
  buf[8] = btohexa_low(col.W);
  buf[9] = 0;
  return String(buf);
}

RgbwColor html2rgbw(String html)
{
  const char* htmlCstr = html.c_str();
  int len = html.length();

  if (*htmlCstr == '#')
  {
    htmlCstr ++;
    len --;
  }
  unsigned long number = strtoul(htmlCstr, NULL, 16);
  
  if (len == 6)
  {
    // #rrggbb
    return RgbwColor((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF, 0);
  }
  else if (len == 8)
  {
    // #rrggbbaa
    return RgbwColor((number >> 24) & 0xFF, (number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);    
  }
  else 
  {
    // Error - return black
    return RgbwColor(0);
  }
}

RgbwColor applyBrightness(RgbwColor in, byte brightness)
{
  return RgbwColor((uint16_t)in.R * brightness / 255, (uint16_t)in.G * brightness / 255, (uint16_t)in.B * brightness / 255,  (uint16_t)in.W * brightness / 255);  
}

RgbwColor scaleRGB(RgbwColor in, byte scale)
{
  return RgbwColor((uint16_t)in.R * scale / 255, (uint16_t)in.G * scale / 255, (uint16_t)in.B * scale / 255, 255 - scale);
}


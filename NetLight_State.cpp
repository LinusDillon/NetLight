#include "NetLight_State.h"
#include "ColorUtil.h"
#include "EspDebug.h"

NetLightState state;

HtmlColor htmlColor1;
HtmlColor htmlColor2;

String nextField(String csv, int* pos)
{
  bool endReached = false;
  espDebug.print("nextField ");
  espDebug.print(*pos);
  if (*pos == -1)
  {
    return "";
  }

  int fieldEnd = csv.indexOf(',', *pos);
  
  espDebug.print(", ");
  espDebug.print(fieldEnd);
  
  if (fieldEnd == -1)
  {
    fieldEnd = csv.length();
    endReached = true;
  }
  String field = csv.substring(*pos, fieldEnd);
  if (endReached)
  {
    *pos = -1;    
  }
  else
  {
    *pos = fieldEnd + 1;    
  }
  espDebug.print(", ");
  espDebug.println(field);
  return field;
}

String NetLightState::toString()
{
  espDebug.println("state toString");

  String csv = String(state.lightEnabled);
  csv += ",";
  csv += state.lightLevel;
  csv += ",";
  csv += rgbw2html(state.color1);
  csv += ",";
  csv += rgbw2html(state.color2);
  csv += ",";
  csv += state.time1_min;
  csv += ",";
  csv += state.time1_max;
  csv += ",";
  csv += state.time2_min;
  csv += ",";
  csv += state.time2_max;
  csv += ",";
  csv += state.lightMode;

  espDebug.println(csv);
  return csv;
}

bool NetLightState::fromString(String csv)
{
  bool effectChanged = false;
  
  espDebug.println("state fromString");
  espDebug.println(csv);
  
  int pos = 0;

  nextField(csv, &pos);
  
  this->lightEnabled = nextField(csv, &pos).toInt();
  espDebug.println(this->lightEnabled);
  
  this->lightLevel = nextField(csv, &pos).toInt();
  espDebug.println(this->lightLevel);
  
  htmlColor1.Parse<HtmlColorNames>(nextField(csv, &pos));
  this->color1 = RgbwColor(htmlColor1);
  espDebug.println(rgbw2html(this->color1));
  
  htmlColor2.Parse<HtmlColorNames>(nextField(csv, &pos));
  this->color2 = RgbwColor(htmlColor2);
  espDebug.println(rgbw2html(this->color2));
  
  this->time1_min = nextField(csv, &pos).toInt();
  espDebug.println(this->time1_min);
  
  this->time1_max = nextField(csv, &pos).toInt();
  espDebug.println(this->time1_max);
  
  this->time2_min = nextField(csv, &pos).toInt();
  espDebug.println(this->time2_min);
  
  this->time2_max = nextField(csv, &pos).toInt();
  espDebug.println(this->time2_max);  

  int lightMode = nextField(csv, &pos).toInt();
  if (lightMode != this->lightMode)
  {
    this->lightMode = lightMode;
    effectChanged = true;
  }
  espDebug.println(this->lightMode);  
}


#include "NetLight_State.h"
#include "ColorUtil.h"
#include "EspDebug.h"

NetLightState state;
NetLightStateAnim animationStates[ANIM_CHANELS];


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

  String csv = String(this->lightEnabled);
  csv += ",";
  csv += this->lightLevel;

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
}

String NetLightState::allAnimToString()
{
  String csv = "";
  for (int i = 0; i < ANIM_CHANELS; i ++)
  {
    csv += i;
    csv += ',';
    csv += animationStates[i].toString();
    csv += "\n";
  }
  return csv;
}

String NetLightState::animToString(int index)
{
  return String(index) + ',' + this->animationStates[index].toString();
}

int NetLightState::animFromString(String csv)
{
  int pos = 0;
  int index = nextField(csv, &pos).toInt();  
  if (this->animationStates[index].fromString(csv, &pos))
  {
    return index;
  }
  return -1;
}
    
String NetLightStateAnim::toString()
{
  String csv = String(rgbw2html(this->color1));
  csv += ",";
  csv += rgbw2html(this->color2);
  csv += ",";
  csv += this->time1_min;
  csv += ",";
  csv += this->time1_max;
  csv += ",";
  csv += this->time2_min;
  csv += ",";
  csv += this->time2_max;
  csv += ",";
  csv += this->lightMode;

  espDebug.println(csv);
  return csv;
}

bool NetLightStateAnim::fromString(String csv, int* pos)
{
  bool effectChanged = false;
  
  this->color1 = html2rgbw(nextField(csv, pos));
  espDebug.println(rgbw2html(this->color1));
  
  this->color2 = html2rgbw(nextField(csv, pos));
  espDebug.println(rgbw2html(this->color2));
  
  this->time1_min = nextField(csv, pos).toInt();
  espDebug.println(this->time1_min);
  
  this->time1_max = nextField(csv, pos).toInt();
  espDebug.println(this->time1_max);
  
  this->time2_min = nextField(csv, pos).toInt();
  espDebug.println(this->time2_min);
  
  this->time2_max = nextField(csv, pos).toInt();
  espDebug.println(this->time2_max);  

  int lightMode = nextField(csv, pos).toInt();
  if (lightMode != this->lightMode)
  {
    this->lightMode = lightMode;
    effectChanged = true;
  }
}


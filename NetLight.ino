/*************************************************** 
  NetLight - Arduino ESP8266 Firmware
  written by Linus Dillon
  using libraries/code from:
  * Limor Fried/Ladyada (Adafruit Industries)
  * https://github.com/esp8266/Arduino
  * https://github.com/tzapu/WiFiManager
  * https://github.com/Makuna/NeoPixelBus
  * https://github.com/bblanchon/ArduinoJson
 ****************************************************/

// includes
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <FS.h>
#include <ArduinoOTA.h>

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WebServer/src/ESP8266WebServer.h
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include <NeoPixelBus.h>          //https://github.com/Makuna/NeoPixelBus
#include <NeoPixelAnimator.h>

#include "NetLight_State.h"
#include "ColorUtil.h"
#include "EspDebug.h"

#define TARGET_NETLIGHT_MK1

#ifdef TARGET_NETLIGHT_PROTO
#define HOSTNAME "NETLIGHT-" ///< Hostname. The setup function adds the Chip ID at the end.
const uint16_t PixelCount = 19; // Star
const uint8_t PixelPin = 2;       // Pin 2, but actually ignored by library for Esp8266
NeoPixelBus<NeoGrbwFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);
#endif

#ifdef TARGET_NETLIGHT_MK1
#define HOSTNAME "NETLIGHT-" ///< Hostname. The setup function adds the Chip ID at the end.
const uint16_t PixelCount = 7; // Jewel
const uint8_t PixelPin = 3;       // Pin 3, but actually ignored by library for Esp8266 // Sculpture
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
#endif

#ifdef TARGET_RUDY
#define HOSTNAME "RUDY-" ///< Hostname. The setup function adds the Chip ID at the end.
//const uint16_t PixelCount = 24 + 16 + 80; // Stick star
const uint16_t PixelCount = 45; // Bottom ring strip
const uint8_t PixelPin = 3;       // Pin 3, but actually ignored by library for Esp8266 // Sculpture
//NeoPixelBus<NeoGrbwFeature, NeoEsp8266Dma800KbpsMethod> strip(PixelCount, PixelPin); // Sculpture
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin); // Sculpture
#endif

#define PASSWORD "abc123"

String deviceHostname(HOSTNAME);
char mqtt_server[40];
char mqtt_port[6] = "1883";
char mqtt_idx[6] = "1";
WiFiManager wifiManager;
ESP8266WebServer server(80);
bool shouldSaveConfig = false;

NeoPixelAnimator animations(ANIM_CHANELS);
RgbwColor black(0);
RgbwColor red(255, 0, 0);
RgbwColor green(0, 255, 0);
RgbwColor blue(0, 0, 255);
RgbwColor currentColour(0, 0, 0, 0);

void setRandomSeed()
{
    uint32_t seed;

    // random works best with a seed that can use 31 bits
    // analogRead on a unconnected pin tends toward less than four bits
    seed = analogRead(0);
    delay(1);

    for (int shifts = 3; shifts < 31; shifts += 3)
    {
        seed ^= analogRead(0) << shifts;
        delay(1);
    }

    randomSeed(seed);
}

void saveConfigCallback () 
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void loadCustomSettings()
{  
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_idx, json["mqtt_idx"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
}

void saveCustomSettings()
{  
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_idx"] = mqtt_idx;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
}

void setup()
{
  Serial.begin(115200);
  strip.Begin();
  
  delay(100);

  Serial.println("\r\n");
  Serial.print("Chip ID: 0x");
  Serial.println(ESP.getChipId(), HEX);

  // Set Hostname.  
  deviceHostname += String(ESP.getChipId(), HEX);
  WiFi.hostname(deviceHostname);

  // Print hostname.
  Serial.println("Hostname: " + deviceHostname);

  // Initialize file system.
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  // Load wifi connection information.
  loadCustomSettings();
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 5);
  WiFiManagerParameter custom_mqtt_idx("id", "mqtt id", mqtt_idx, 5);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_idx);
  strip.ClearTo(blue);
  strip.Show();
  if(!wifiManager.autoConnect((const char *)deviceHostname.c_str(), PASSWORD)) {
    strip.ClearTo(red);
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    WiFi.softAP((const char *)deviceHostname.c_str(), PASSWORD);
    //ESP.reset();
    //delay(1000);
  }  
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(mqtt_idx, custom_mqtt_idx.getValue());

  Serial.println("on network");
  Serial.println(WiFi.localIP());

  espDebug.start();
 
  espDebug.println("debug started");

  // Start OTA server.
  ArduinoOTA.setHostname((const char *)deviceHostname.c_str());
  ArduinoOTA.begin();

  espDebug.println("ota started");

  // Add services to MDNS-SD
  MDNS.addService("http", "tcp", 80);
  webServerSetup();

  espDebug.println("mdns/web server started");

  if (shouldSaveConfig) {
    saveCustomSettings();
  }

  setRandomSeed();

  // Resets all neopixels to an off state
  strip.ClearTo(green);
  strip.Show();
  delay(500);
  strip.ClearTo(black);
  strip.Show();
  espDebug.println("neopixel started");

  // Initial animation state
  state.animationStates[0].lightMode = LIGHTMODE_FIXED;
}

void loop()
{
  // Handle debug
  espDebug.handle();
  
  // Handle OTA server.
  ArduinoOTA.handle();

  // Handle web server
  server.handleClient();



  if (state.lightEnabled)
  {
    // Update animation
    for (int i = 0; i < ANIM_CHANELS; i ++)
    {
      if (!animations.IsAnimationActive(i))
      {
        StartStateAnimation(i);
      }
    }
    animations.UpdateAnimations();

    ApplyBrightnessToStrip();
    strip.Show();
  }
  else
  {
    strip.ClearTo(black);
    strip.Show();
  }
    
  yield();
}


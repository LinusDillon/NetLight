#ifndef HEADER_ESP_DEBUG
#define HEADER_ESP_DEBUG

#include <ESP8266WiFi.h>

#define MAX_ESP_DEBUG_CLIENTS 1


class EspDebug : public Print
{
  WiFiServer server = WiFiServer(23);
  WiFiClient serverClients[MAX_ESP_DEBUG_CLIENTS];

  public:
    void start();
    void handle();
    virtual size_t write(uint8_t);
    virtual size_t write(const char *str);
    virtual size_t write(const uint8_t *buffer, size_t size);
};

extern EspDebug espDebug;

#endif

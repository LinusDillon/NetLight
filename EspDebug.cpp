#include "EspDebug.h"

EspDebug espDebug;

void EspDebug::start()
{
  server.begin();
  server.setNoDelay(true);
}

void EspDebug::handle()
{
  uint8_t i;
  
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_ESP_DEBUG_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial1.print("New client: "); Serial1.print(i);
        continue;
      }
    }
    
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
}

size_t EspDebug::write(uint8_t character) 
{
  uint8_t i;

  //push UART data to all connected telnet clients
  Serial.write(character);
  for(i = 0; i < MAX_ESP_DEBUG_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      serverClients[i].write(character);
      delay(1);
    }
  }
}

size_t EspDebug::write(const char *str) 
{
  uint8_t i;

  //push UART data to all connected telnet clients
  Serial.write(str);
  for(i = 0; i < MAX_ESP_DEBUG_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      serverClients[i].write(str);
      delay(1);
    }
  }
}

size_t EspDebug::write(const uint8_t *buffer, size_t size) 
{
  uint8_t i;

  //push UART data to all connected telnet clients
  Serial.write(buffer, size);
  for(i = 0; i < MAX_ESP_DEBUG_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      serverClients[i].write(buffer, size);
      delay(1);
    }
  }
}


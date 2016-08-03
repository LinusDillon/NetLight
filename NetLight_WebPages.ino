void handleGetInfo()
{  
  String message = "Info,";
  message += mqtt_server;
  message += ",";
  message += mqtt_port;
  message += ",";
  message += mqtt_idx;
  message += ",";
  message += ESP.getFlashChipSizeByChipId();
  message += ",";
  message += ESP.getFlashChipSize();
  message += ",";
  message += ESP.getFreeHeap();
  message += ",";
  message += ESP.getSketchSize();
  message += ",";
  message += ESP.getFreeSketchSpace();
  server.send(200, "text/csv", message);
}

void sendStatusResponse()
{  
  String message = "State,";
  message += state.toString();
  server.send(200, "text/csv", message);
}

void handleGetStatus() 
{
  espDebug.println("get status");
  sendStatusResponse();
}

void handlePutStatus() 
{
  String csv = server.arg("plain");
  if (state.fromString(csv))
  {
    StartStateAnimation();
  }
  sendStatusResponse();
}

void handleNotFound()
{
  if (handleFileRead(server.uri()))
    return;

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){
  if(path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleResetWifi()
{
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "OK");
  delay(1000);
  wifiManager.resetSettings();
  ESP.reset();
  delay(5000);
}

//void handleColor()
//{
//  String colArg;
//  
//  if (server.hasArg("rgb"))
//  {
//    colArg = server.arg("rgb");
//    unsigned long number = strtoul(colArg.c_str(), NULL, 16);
//    state.color1 = RgbwColor((number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF, 0);
//  }
//  else if (server.hasArg("rgbw"))
//  {
//    colArg = server.arg("rgbw");
//    unsigned long number = strtoul(colArg.c_str(), NULL, 16);
//    state.color1 = RgbwColor((number >> 24) & 0xFF, (number >> 16) & 0xFF, (number >> 8) & 0xFF, number & 0xFF);    
//  }
//  else if (server.hasArg("hsl"))
//  {
//    colArg = server.arg("hsl");    
//    unsigned long number = strtoul(colArg.c_str(), NULL, 16);
//    unsigned char h = (number >> 16) & 0xFF;
//    float s = (float)((number >> 8) & 0xFF) / (float)255;
//    float l = (float)(number & 0xFF) / (float)255;
//    state.color1 = hsi2rgbw(h, s, l);    
//  }
//  else
//  {
//    server.send(400, "text/plain", "Expeceted rgb, rgbw, or hsl argument.");
//    return;
//  }
//  redirectToRoot();
//}

void redirectToRoot()
{
  server.sendHeader("Location", "/");
  server.send(303, "text/plain", "OK");  
}

void webServerSetup()
{
  server.on("/info", handleGetInfo);
  server.on("/status", HTTP_GET, handleGetStatus);
  server.on("/status", HTTP_PUT, handlePutStatus);
  server.on("/resetwifi", handleResetWifi);
  server.onNotFound(handleNotFound);
  server.begin();
}


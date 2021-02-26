#include <Arduino.h>

// WiFi stuff
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>

// OTA stuff
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Camera stuff
#include "OV2640.h"
#include "OV2640Streamer.h"


// Button stuff
#include <OneButton.h>

#define ENABLE_WEBSERVER1
#define ENABLE_WEBSERVER2


// Camera class
extern OV2640 cam;



// Web server stuff
void initWebStream1(void);
void initWebStream2(void);
void stopWebStream(void);
//void stopWebStream(void);
void handleWebServer(void);

// OTA stuff
void startOTA(void);
void stopOTA(void);
extern boolean otaStarted;
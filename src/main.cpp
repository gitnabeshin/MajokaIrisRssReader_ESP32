/************************************************************************
 * ESP32 Yahoo News viewer with Majoka IRIS long display. 
 * **********************************************************************/
#if defined(ESP8266)
#include <ESP8266WiFi.h>            //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>                   //https://github.com/esp8266/Arduino
#endif

//needed for library
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>            //https://github.com/tzapu/WiFiManager

#include "LCD_Viewer.h"

// Display message for switching to AP Mode
void switchToAPMode( WiFiManager* pWiFiManager ){
    LCD_Viewer_drawString("      Please Setup WiFi.        SSID:AutoConnectAP, http://192.168.4.1 ", LCD_TEXT_COLOR_BLUE);
}

void setup() {

    Serial.begin(115200);
    delay(500);
    LCD_Viewer_setup();

    WiFiManager wifiManager;

    // Should be callbacked
    LCD_Viewer_drawString("                ESP32  Yahoo News Viewer      ", LCD_TEXT_COLOR_RED);

    // Set callback function to WiFiManager(AP Mode switching)
    wifiManager.setAPCallback( switchToAPMode );

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    if(!wifiManager.autoConnect("AutoConnectAP")) {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(5000);
    } 

    Serial.println("SSID: " + WiFi.SSID() + "  IP: " + WiFi.localIP().toString() + "connected.");
    // LCD_Viewer_drawString("SSID: " + WiFi.SSID() + "  IP: " + WiFi.localIP().toString());
    LCD_Viewer_drawString("   WiFi connected.                  SSID: " + WiFi.SSID() + "  ", LCD_TEXT_COLOR_BLUE);
}

void loop() {
    LCD_Viewer_drawRSSString();
}
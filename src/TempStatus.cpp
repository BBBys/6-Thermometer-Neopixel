/**
 * @file TempStatus.cpp
 * @brief Temperaturen im ganzen Haus anzeigen
 * @version 1.2
 * @date 20 Feb 15 14 13 Jan 2022
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2022 B. Borys
 * Info zu Neopixel: https://learn.adafruit.com/adafruit-neopixel-uberguide/
 */
#include "TempStatus.h"

Ticker Zeitgeber;
extern EspMQTTClient client;

float Temp[nTemps] = {0, 0, 0, 0, 0, 0, 0};
int nStrips = nTemps - 1; // Heizung benötigt 2

/**
 * @brief setting up the NeoPixel library
 * When setting up the NeoPixel library, we tell it how many pixels, and which pin to use to send signals. 
 * Note that for older NeoPixel strips you might need to change the third parameter -- see the strandtest example for more information on possible values.
 */
Adafruit_NeoPixel pixels(iNUMPIXELS*nStrips, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  String bitmap[10] = {
      "......",
      "..rr..",
      ".rwwr.",
      "rwwwwr",
      ".rwwr.",
      "..rr..",
      "......",
      "......",
      "......",
      "......"};
  pinMode(LED_BUILTIN, OUTPUT);
  Zeitgeber.attach(.5, flip);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(25);
  Bild(&pixels, 6, 10, bitmap);
#ifndef NDEBUG
  Serial.begin(115200);
  Serial.println("setup");
#endif
#ifdef NDEBUG
  client.enableDebuggingMessages(false);
#else
  client.enableDebuggingMessages(true);
#endif
  client.enableHTTPWebUpdater();                                               
  client.enableLastWillMessage("ThermoAnzeige/lastwill", "Abbruch ThermoAnzeige"); /// LWT-Meldung
  }

void loop() {
  client.loop();
  //if (!client.isConnected () )Serial.println("!connected");
  //if (!client.isWifiConnected () )Serial.println("!WIFIconnected");
  //if (!client.isMqttConnected () )Serial.println("!MQTTconnected");
}
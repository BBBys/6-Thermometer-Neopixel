/**
 * @file TSMqtt.cpp
 * @brief MQTT für "Temperaturen im ganzen Haus anzeigen"
 * @version 1.2
 * @date 20 Feb 26 16 15 Jan 2022
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2022 B. Borys
 */
#include "TempStatus.h"
extern float Temp[nTemps];
extern Ticker Zeitgeber;
extern Adafruit_NeoPixel pixels;
/*
   ================================================================================================
   MQTT
*/
EspMQTTClient client(
    WLANSSID,
    WLANPWD,
    MQTTBROKERIP, // MQTT Broker server ip
    OTAUSER,      // Can be omitted if not needed
    OTAPASSWD,    // Can be omitted if not needed
    MQTTNAME,     // Client name that uniquely identify your device
    1883          // The MQTT port, default to 1883. this line can be omitted
);
/*
   MQTT
   ====================================================================================================
*/

/**
 * @brief MQTT-Callback
 * 
 */
void onConnectionEstablished()
{
    String bitmap1[10] = {
        "..y...",
        ".yyy..",
        "yyyyy.",
        "..y...",
        "..y...",
        "..y...",
        "......",
        "......",
        "......",
        "......"};
    String bitmap2[10] = {
        "..y...",
        ".yyy..",
        "yyyyy.",
        "..y...",
        "..yg..",
        "..yg..",
        "...g..",
        ".ggggg",
        "..ggg.",
        "...g.."};

    //log_d("Start...");
    Bild(&pixels, 6, 10, bitmap1);
    client.subscribe("Temp/Wohnzimmer", [](const String &payload)
                     {
                         //Serial.println(payload);
                         Temp[tWz] = payload.toDouble();
                         //Serial.println(Temp[tWz]);
                         Anzeigen(); });
    client.subscribe("Temp/Bad", [](const String &payload)
                     {
                         //Serial.println(payload);
                         Temp[tBad] = payload.toDouble();
                         //Serial.println(Temp[tWz]);
                         Anzeigen();
                     });
    client.subscribe("Temp/DG", [](const String &payload)
                     {
                         //Serial.println(payload);
                         Temp[tDG] = payload.toDouble();
                         //Serial.println(Temp[tWz]);
                         Anzeigen();
                     });
    client.subscribe(ZIMMER1, [](const String &payload)
                     {
                         //Serial.println(payload);
                         Temp[tJo] = payload.toDouble();
                         //Serial.println(Temp[tWz]);
                         Anzeigen();
                     });
    client.subscribe(ZIMMER2, [](const String &payload)
                     {
                         //Serial.println(payload);
                         Temp[tCl] = payload.toDouble();
                         //Serial.println(Temp[tWz]);
                         Anzeigen();
                     });
    client.subscribe("Vorlauf", [](const String &payload)
                     {
                         Serial.println(payload);
                         Temp[tHzV] = payload.toDouble();
                         //Serial.println(Temp[tWz]);
                         Anzeigen();
                     });
    client.subscribe("Ruecklauf", [](const String &payload)
                     {
                         Serial.println(payload);
                         Temp[tHzR] = payload.toDouble();
                         //Serial.println(Temp[tWz]);
                         Anzeigen();
                     });
    // client.subscribe("Temp/Aussen", [](const String &payload)
    //                  {
    //                    //Serial.println(payload);
    //                    TempA = payload.toDouble();
    //                    //Serial.println(TempA);
    //                    Anzeigen();
    //                  });
    client.publish("SWVersion", VERSION);
    client.publish("SWDatum", __DATE__);
    client.publish("OTA-Usr", OTAUSER);
    client.publish("OTA-Pwd", OTAPASSWD);
    client.publish("Status", "Anzeige läuft");
    Bild(&pixels, 6, 10, bitmap2);
    Zeitgeber.detach();
    Zeitgeber.attach(WARTEZEIT, cbWartezeit);
} // void onConnectionEstablished()

/**
 * @file TempStatus.h
 * @brief Temperaturen im ganzen Haus anzeigen
 * @version 1.3
 * @date 26 20 Feb 29 Jan 2022
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2022 B. Borys
 */
#ifndef TEMPSTATUS_H
#define TEMPSTATUS_H

#include <Arduino.h>
#include "EspMQTTClient.h"
#include <Adafruit_NeoPixel.h>
#include <Ticker.h> /// Load library "ticker" for blinking status led

#define NEOPIXELPIN 2
#define iNUMPIXELS 10 /// 10 Pixel je Temperaturanzeige
#define WARTEZEIT 660 /// Update soll alle 10 Minuten sein, nach 11 Minuten Warnanzeige

/**
 * @brief Farben als RGB-Werte
 */
enum RGBFarben
{
    NEOROT = 0xff0000,
    NEOGELB = 0xff5F00,
    NEOGRUEN = 0x00ff00,
    NEOBLAU = 0x0000ff,
    NEOWEISS = 0xffffff
};

// #define HUEGRUEN 21845
// #define HUEBLAU 43690
// #define HUEROT 0
enum HueFarben_t
{
    HUEROT = 0,
    HUEGELB = 10923U,          // 65336 * 1/6
    HUEGELBLICHGRUEN = 18204U, // grün + 1/3 gelb
    HUEGRUEN = 21845U,         // 65336 * 2/6
    HUECYAN = 32768U,          // 65336 * 3/6
    HUEBLAU = 43691U,          // 65336 * 4/6
    HUEMAGENTA = 54613U,       // 65336 * 5/6
    HUEROT2 = 65335U           // 65336 * 6/6
};
enum Temps
{
    tWz,
    tJo,
    tCl,
    tDG,
    tBad,
    tHzV,
    tHzR,
    nTemps
};
void Bild(Adafruit_NeoPixel *pixels, uint8_t spalten, uint8_t zeilen, String bitmap[]);
void Anzeigen();
void cbWartezeit();
void cbBlink(HueFarben_t pFarbton);
void flip();
double dmap(double x, double in_min, double in_max, double out_min, double out_max);
void AnzeigenI(uint8_t pNummer, float pTemp);
void Anzeigen2H(uint8_t pNummer, float pTempV, float pTempR);

#endif
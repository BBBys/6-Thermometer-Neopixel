/**
 * @file TSHilfen.cpp
 * @brief Hilfen für "Temperaturen im ganzen Haus anzeigen"
 * @version 1.2
 * @date 20 19 Feb 29 28 26 14 Jan 2022
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2022 B. Borys
 */
#include "TempStatus.h"
int nFlip = 0;
// extern const int nStrips;
extern int nStrips;
extern Adafruit_NeoPixel pixels;
extern Ticker Zeitgeber;
extern float Temp[nTemps];
/**
 * @brief Innentemperatur auf einem Strip anzeigen
 * der eine Länge von iNUMPIXELS (10) LEDs hat. der Bereich 19...21 wird gespreizt
 * @param pNummer Nummer des Strips, 0...(nStrips-1), jeder hat iNUMPIXELS (10) LEDs
 * @param pTemp Temperatur, °C
 */
void AnzeigenI(int pNummer, float pTemp)
{ /// Bereiche Innen
    const float iBLAUBIS = 18.0;
    const float iGRUENAB = 19.75;
    const float iGRUENBIS = 21.25;
    const float iROTAB = 26.0;
    /// Anzeigebereich, niedrighste und höchste Temperatur, die aufgelöst wird
    const int iTMIN = (+17), iTMAX = (+27);
    /// Zielbereich, der Temperaturbereich wird gespreizt dargestellt
    const int SOLLMIN = (+19), SOLLMAX = (+22);
    /// Pixel, in denen der Zielbereich dargestellt wird
    const int PIXMIN = (2), PIXMAX = (iNUMPIXELS - 4);
    float tBegrenzt, fPixel, dezimalPixel;
    int intPixel, cFarbton, cRGB;
    // Pixel aus
    pixels.fill(0, pNummer * iNUMPIXELS, iNUMPIXELS);
    tBegrenzt = constrain(pTemp, iTMIN, iTMAX);
    // temperaturahängigen Farbton berechnen
    cFarbton = HUEGRUEN; // grün, wenn nichts anderes
    if (pTemp < iGRUENAB)
        cFarbton = dmap(pTemp, iBLAUBIS, iGRUENAB, HUEBLAU, HUEGRUEN); // kälter als grün: Blautöne bis grün
    if (pTemp < iBLAUBIS)
        cFarbton = HUEBLAU; // noch kälter: blau
    if (pTemp > iGRUENBIS)
        cFarbton = dmap(pTemp, iGRUENBIS, iROTAB, HUEGELBLICHGRUEN, HUEROT); // wärmer als grün: grün ... gelb bis rot
    if (pTemp > iROTAB)
        cFarbton = HUEROT; // noch wärmer: rot

    /// Umrechnung der Temperatur auf nichtlineare Skala
    if (pTemp < SOLLMIN)
        fPixel = dmap(tBegrenzt, iTMIN, SOLLMIN, 0, PIXMIN);
    else if (pTemp < SOLLMAX)
        fPixel = dmap(tBegrenzt, SOLLMIN, SOLLMAX, PIXMIN, PIXMAX);
    else
        fPixel = dmap(tBegrenzt, SOLLMAX, iTMAX, PIXMAX, iNUMPIXELS - 1);

    intPixel = (int)fPixel;                  // ganzzahliger Teil, bis zu diesem Pixel wird die Skala gefüllt
    dezimalPixel = fPixel - (float)intPixel; // Dezimalstellen, damit wird die Helligkeit des nächst folgenden Pixels eingestellt
    if (intPixel < iNUMPIXELS - 1)
    {
        float fhell;
        int ihell;
        fhell = 255.0 * dezimalPixel;
        ihell = (int)(fhell + 0.5);
        cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, ihell));
        pixels.setPixelColor(pNummer * iNUMPIXELS + intPixel + 1, cRGB);
        cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, 255));
        pixels.fill(cRGB, pNummer * iNUMPIXELS, intPixel + 1);
    }
    else // wenn wir ganz oben angekommen sind
    {
        cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, 255));
        pixels.fill(cRGB, pNummer * iNUMPIXELS, intPixel + 1);
    }
}
/**
 * @brief Heizungstemperatur auf einem Strip anzeigen
 *
 * @param pNummer Nummer des Strips, 0...(nStrips-1)
 * @param pTemp Temperatur, °C
 */
void AnzeigenH(int pNummer, float pTemp, bool loeschen)
{
    /// Bereiche Heizung
    const float iBLAUBIS = 30;
    const float iGRUENAB = 42;
    const float iGRUENBIS = 43;
    const float iROTAB = 52;
    const float iROTBIS = 53;
    const float iMAGENTAAB = 60;
    const int iTMIN = (+30);
    const int iTMAX = (+65);
#ifndef NDEBUG
    Serial.printf("Heizung %f", pTemp);
#endif
    // uint8_t sat = 255, bright = 75;
    float tBegrenzt, dezimalpixel, fPixel;
    int intPixel, zielPixel, cFarbton, cRGB;
    if(loeschen)pixels.fill(0, pNummer * iNUMPIXELS, iNUMPIXELS);
    tBegrenzt = constrain(pTemp, iTMIN, iTMAX);
    // Serial.println(tBegrenzt);
    fPixel = dmap(tBegrenzt, iTMIN, iTMAX, 0, iNUMPIXELS - 1);
    // Serial.println(fPixel);
    intPixel = (int)fPixel;                  // ganzzahliger Teil
    dezimalpixel = fPixel - (float)intPixel; // Dezimalstellen
    // Serial.println(intPixel);
    // Serial.println(dezimalpixel);
    cFarbton = HUEGRUEN; // grün
    if (pTemp < iGRUENAB)
        cFarbton = dmap(pTemp, iBLAUBIS, iGRUENAB, HUEBLAU, HUEGRUEN); // blau bis grün
    if (pTemp < iBLAUBIS)
        cFarbton = HUEBLAU; // blau
    if (pTemp > iGRUENBIS)
        cFarbton = dmap(pTemp, iGRUENBIS, iROTAB, HUEGRUEN, HUEROT); // grün bis rot
    if (pTemp > iROTAB)
        cFarbton = HUEROT; // rot
    if (pTemp > iROTBIS)
        cFarbton = dmap(pTemp, iROTBIS, iMAGENTAAB, HUEROT2, HUEMAGENTA); 
    if (pTemp > iMAGENTAAB)
        cFarbton = HUEMAGENTA; 
    zielPixel = intPixel + pNummer * iNUMPIXELS;
    if (intPixel < iNUMPIXELS - 1)
    {
        float fhell;
        int ihell;
        fhell = 255.0 * dezimalpixel;
        ihell = (int)(fhell + 0.5);
        cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, ihell));
        pixels.setPixelColor(zielPixel + 1, cRGB);
        ihell = 255 - ihell;
        // cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, ihell));
        cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, 255));
        // pixels.setPixelColor(zielPixel, cRGB);
        pixels.fill(cRGB, pNummer * iNUMPIXELS, intPixel + 1);
    }
    else
    {
        cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, 255));
        // pixels.setPixelColor(zielPixel, cRGB);
        pixels.fill(cRGB, pNummer * iNUMPIXELS, intPixel + 1);
    }
    // Serial.println(cFarbton);
    // Serial.println(intPixel);
}

void Anzeigen()
{
    Zeitgeber.detach(); /// Timeout-Timer aus
    pixels.clear();     // Set all pixel colors to 'off'
    AnzeigenI(tWz, Temp[tWz]);
    AnzeigenI(tBad, Temp[tBad]);
    AnzeigenI(tDG, Temp[tDG]);
    AnzeigenI(tJo, Temp[tJo]);
    AnzeigenI(tCl, Temp[tCl]);
    AnzeigenH(tHzV, Temp[tHzV],true);
    AnzeigenH(tHzV, Temp[tHzR],false);
    pixels.show();                            // Send the updated pixel colors to the hardware.
    Zeitgeber.attach(WARTEZEIT, cbWartezeit); /// Timeout-Timer starten
}

/**
 * @brief Timer-Callback: Wartezeit abgelaufen, rotes Blinken einschalten
 *
 */
void cbWartezeit()
{
    Zeitgeber.detach();
    Zeitgeber.attach(1, cbBlink, HUEROT);
}

/**
 * @brief Timer-Callback für laufendes Licht entlang der Kette
 *
 * @param pFarbton Hue
 */
void cbBlink(HueFarben_t pFarbton)
{
    int intPixel = (nFlip++) % (iNUMPIXELS * nStrips);
    pixels.clear(); // Set all pixel colors to 'off'
    pixels.setPixelColor(intPixel, pixels.ColorHSV(pFarbton, 255, 255));
    pixels.show(); // Send the updated pixel colors to the hardware.
}

/**
 * @brief bessere Mapping-Funktion
 *
 * @param x Eingangswert im Bereich in_min...in_max
 * @param in_min
 * @param in_max
 * @param out_min
 * @param out_max
 * @return double Ausgangswert im Bereich out_min...out_max
 */
double dmap(double x, double in_min, double in_max, double out_min, double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/**
 * @brief interne LED umschalten
 *
 */
void flip()
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // set pin to the opposite state
}

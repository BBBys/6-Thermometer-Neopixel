/**
 * @file TSHilfen.cpp
 * @brief Hilfen für "Temperaturen im ganzen Haus anzeigen"
 * @version 1.3
 * @date 26 25 24 20 19 Feb 29 28 26 14 Jan 2022
 * @author Dr. Burkhard Borys, Zeller Ring 15, 34246 Vellmar, Deutschland
 * @copyright Copyright (c) 2022 B. Borys
 */
#include "TempStatus.h"
uint8_t nFlip = 0;
extern uint8_t nStrips;
extern Adafruit_NeoPixel pixels;
extern Ticker Zeitgeber;
extern float Temp[nTemps];
/**
 * @brief Innentemperatur auf einem Strip anzeigen
 * der eine Länge von iNUMPIXELS (10) LEDs hat. der Bereich 19...21 wird gespreizt
 * @param pNummer Nummer des Strips, 0...(nStrips-1), jeder hat iNUMPIXELS (10) LEDs
 * @param pTemp Temperatur, °C
 */
void AnzeigenI(uint8_t pNummer, float pTemp)
{
    const float iBLAUBIS = 18.0;
    const float iGRUENAB = 19.75;
    const float iGRUENBIS = 21.0;
    const float iROTAB = 26.0;
    /// Anzeigebereich, niedrighste und höchste Temperatur, die aufgelöst wird
    const uint8_t iTMIN = (+17), iTMAX = (+27);
    /// Zielbereich, der Temperaturbereich wird gespreizt dargestellt
    const uint8_t SOLLMIN = (+19), SOLLMAX = (+22);
    /// Pixel, in denen der Zielbereich dargestellt wird
    const uint8_t PIXMIN = (2), PIXMAX = (iNUMPIXELS - 4);
    float tBegrenzt, fPixel, dezimalPixel;
    uint8_t intPixel;
    uint16_t cFarbton;
    uint32_t cRGB; /// muss 32 Bit sein denn (R,G,B)*8 sind schon 24
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
        cFarbton = dmap(pTemp, iGRUENBIS, iROTAB, HUEGRUEN, HUEROT); // wärmer als grün: grün ... gelb bis rot
    if (pTemp > iROTAB)
        cFarbton = HUEROT; // noch wärmer: rot
    if (pTemp < SOLLMIN)                                                  // Temperatur bis 19
        fPixel = dmap(tBegrenzt, iTMIN, SOLLMIN, 0, PIXMIN);              // Pixel von 0.0 bis 2.0
    else if (pTemp < SOLLMAX)                                             // Temperatur von 19 bis 22
        fPixel = dmap(tBegrenzt, SOLLMIN, SOLLMAX, PIXMIN, PIXMAX);       // Pixel von 2.0 bis 6.0
    else                                                                  // Temperatur ab 22
        fPixel = dmap(tBegrenzt, SOLLMAX, iTMAX, PIXMAX, iNUMPIXELS - 1); // Pixel von 6.0 bis 9.0
    intPixel = (uint8_t)fPixel;              // ganzzahliger Teil, bis zu diesem Pixel wird die Skala gefüllt
    dezimalPixel = fPixel - (float)intPixel; // Dezimalstellen, damit wird die Helligkeit des nächst folgenden Pixels eingestellt
#ifndef NDEBUG
    Serial.printf("Temp=%f fp=%f ip=%d dp=%f\n", pTemp, fPixel, intPixel, dezimalPixel);
#endif
    if (intPixel < iNUMPIXELS - 1) //
    {
        float fhell;
        uint8_t ihell;
        fhell = 255.0 * dezimalPixel;
        ihell = (uint8_t)(fhell + 0.5);
        cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, ihell));
        pixels.setPixelColor(pNummer * iNUMPIXELS + intPixel, cRGB);
        cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, 255));
        if (intPixel > 0)
            pixels.fill(cRGB, pNummer * iNUMPIXELS, intPixel); ///@note oder +1?
        else
            pixels.setPixelColor(pNummer * iNUMPIXELS, cRGB); // ist so richtig
#ifndef NDEBUG
        Serial.printf("Temp=%f Farbe=%d RGB=%06x\n", pTemp, cFarbton, cRGB);
#endif
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
 * @param pTempV Vorlauftemperatur, °C
 * @param pTempR Rücklauftemperatur, °C
 */
void Anzeigen2H(uint8_t pNummer, float pTempV, float pTempR)
{
    /// Bereiche Heizung
    const float iBLAUBIS = 31;
    const float iGRUENAB = 40;
    const float iGRUENBIS = 41;
    const float iROTAB = 52;
    const float iROTBIS = 53;
    const float iMAGENTAAB = 60;
    const uint8_t iTMIN = (+30);
    const uint8_t iTMAX = (+70);
#ifndef NDEBUG
    Serial.printf("Heizung %f, %f;", pTempV, pTempR);
#endif
    // uint8_t sat = 255, bright = 75;
    float tBegrenzt[2];
    pixels.fill(0, pNummer * iNUMPIXELS, iNUMPIXELS);
    tBegrenzt[0] = constrain(pTempV, iTMIN, iTMAX);
    tBegrenzt[1] = constrain(pTempR, iTMIN, iTMAX);
    // Serial.println(tBegrenzt);
    for (size_t i = 0; i < 2; i++)
    {
        uint8_t intPixel;
        float dezimalpixel, fPixel;
        uint16_t cFarbton;
        uint32_t cRGB;
        fPixel = dmap(tBegrenzt[i], iTMIN, iTMAX, 0, iNUMPIXELS - 1);
        intPixel = (uint8_t)fPixel;              // ganzzahliger Teil
        dezimalpixel = fPixel - (float)intPixel; // Dezimalstellen
#ifndef NDEBUG
        Serial.printf("begrenzt %f\nfPixel=%f,intPixel=%d,dezimal=%f\n",tBegrenzt[i], fPixel,intPixel,dezimalpixel);
#endif
        cFarbton = HUEGRUEN; // grün
        if (tBegrenzt[i] < iGRUENAB)
            cFarbton = dmap(tBegrenzt[i], iBLAUBIS, iGRUENAB, HUEBLAU, HUEGRUEN); // blau bis grün
        if (tBegrenzt[i] < iBLAUBIS)
            cFarbton = HUEBLAU; // blau
        if (tBegrenzt[i] > iGRUENBIS)
            cFarbton = dmap(tBegrenzt[i], iGRUENBIS, iROTAB, HUEGRUEN, HUEROT); // grün bis rot
        if (tBegrenzt[i] > iROTAB)
            cFarbton = HUEROT; // rot
        if (tBegrenzt[i] > iROTBIS)
            cFarbton = dmap(tBegrenzt[i], iROTBIS, iMAGENTAAB, HUEROT2, HUEMAGENTA);
        if (tBegrenzt[i] > iMAGENTAAB)
            cFarbton = HUEMAGENTA;
        if (intPixel < iNUMPIXELS - 1)
        {
            float fhell;
            uint8_t ihell;
            fhell = 255.0 * dezimalpixel;
            ihell = (uint8_t)(fhell + 0.5);
            cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, ihell));
            pixels.setPixelColor(pNummer * iNUMPIXELS + intPixel+1, cRGB);
            cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, 255-ihell));
            pixels.setPixelColor(pNummer * iNUMPIXELS+intPixel, cRGB); // ist so richtig
        }
        else
        {
            cRGB = pixels.gamma32(pixels.ColorHSV(cFarbton, 255, 255));
            pixels.setPixelColor(pNummer * iNUMPIXELS + intPixel , cRGB); // ist so richtig
        }
    }
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
    Anzeigen2H(tHzV, Temp[tHzV], Temp[tHzR]);
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
    uint8_t intPixel = (nFlip++) % (iNUMPIXELS * nStrips);
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

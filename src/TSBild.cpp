/**
 * @file TSBild.cpp
 * @author BBB
 * @brief Bild auf Neopixel-Streifen anzeigen
 * @version 1.0
 * @date 20 14 Feb 2022
 * @copyright Copyright (c) 2022 B. Borys
 * @note in Arbeit
 */
#include "TempStatus.h"
/**
 * @brief ein Bild darstellen
 * 
 * @param pNeoPixels 
 * @param nSpalten 
 * @param nZeilen 
 * @param sBild 
 */
void Bild(Adafruit_NeoPixel *pNeoPixels, uint8_t nSpalten, uint8_t nZeilen, String sBild[])
{
    const uint8_t nz = 10, ns = 6;
    const uint8_t idx[nz][ns] =
        {
            {9, 19, 29, 39, 49, 59},
            {8, 18, 28, 38, 48, 58},
            {7, 17, 27, 37, 47, 57},
            {6, 16, 26, 36, 46, 56},
            {5, 15, 25, 35, 45, 55},
            {4, 14, 24, 34, 44, 54},
            {3, 13, 23, 33, 43, 53},
            {2, 12, 22, 32, 42, 52},
            {1, 11, 21, 31, 41, 51},
            {0, 10, 20, 30, 40, 50}};
    // Serial.println("Bild");
    for (uint8_t i = 0; i < nZeilen; i++)
    {
        // Serial.printf("i=%d ",i);
        char z[nSpalten+1];  // warum steht hier +1? aber anders geht es nicht
        sBild[i].toCharArray(z, nSpalten+1);
        for (uint8_t  j = 0; j < nSpalten; j++)
        {
            // Serial.printf("j=%d\n",j);
            uint8_t p;
            p = idx[i][j];
            // Serial.printf("p=%d\n",p);
            switch (z[j])
            {
            case 'r':
                pNeoPixels->setPixelColor(p, NEOROT);
                break;
            case 'g':
                pNeoPixels->setPixelColor(p, NEOGRUEN);
                break;
            case 'w':
                pNeoPixels->setPixelColor(p, NEOWEISS);
                break;
            case 'y':
                pNeoPixels->setPixelColor(p, NEOGELB);
                break;
            default:
                pNeoPixels->setPixelColor(p, 0);
                break;
            }
        }
    }
    pNeoPixels->show();
    // delay(2222);
    // Serial.println("Ende Bild");
}
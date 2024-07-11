#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "qrcode.h"

extern MCUFRIEND_kbv tft;

extern void setupDisplay();
extern void displayQRCode(QRCode* qrcode, int version);

#endif
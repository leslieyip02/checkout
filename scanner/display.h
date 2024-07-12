#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "qrcode.h"

#define TIMER_INACTIVE -1

extern MCUFRIEND_kbv tft;

extern void setupDisplay();
extern void clearScreen();
extern void displayQRCode(QRCode* qrcode, int version);
extern void displayTimerBar();
extern void updateTimerBar();
extern void displayHorizontallyCenteredText(char* text, int y);
extern void displayAmount(double amount);

#endif
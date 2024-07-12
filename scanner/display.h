#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "qrcode.h"

#define TIMER_INACTIVE -1
#define IDLE_TEXT_INACTIVE -1

extern MCUFRIEND_kbv tft;

extern void setupDisplay();
extern void clearScreen(int color);
extern void displayHorizontallyCenteredText(char* text, int y);
extern void updateScreen();

// QR code screen
extern void displayPaymentScreen(double amount);
void displayQRCode(QRCode* qrcode, int version);
void displayTimerBar();
void updateTimerBar();
void displayAmount(double amount);

// idle screen (bouncing DVD logo)
extern void displayIdleScreen();
void toggleIdleTextColor();
void updateIdleText();

#endif
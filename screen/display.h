#ifndef DISPLAY_H
#define DISPLAY_H

#define TIMER_INACTIVE -1
#define IDLE_TEXT_INACTIVE -1

extern void setupDisplay();
extern void clearScreen(int color);
extern void displayHorizontallyCenteredText(char* text, int y);
extern void updateScreen();

// QR code screen
extern void displayPaymentScreen(double amount);
void displayQRCode(char* code);

void displayTimerBar();
void updateTimerBar();
void displayAmount(double amount);

// idle screen (bouncing DVD logo)
extern void displayIdleScreen();
void toggleIdleTextColor();
void updateIdleText();

#endif
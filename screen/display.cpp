#include <Adafruit_GFX.h>
#include <avr/pgmspace.h>
#include <MCUFRIEND_kbv.h>
#include <qrcode.h>

#include "config.h"
#include "display.h"
#include "paynow.h"
#include "record.h"

constexpr int QRCodeWidth = (4 * QR_VERSION + 17) * QR_PIXEL_SIZE;
constexpr int padding = (SCREEN_WIDTH - QRCodeWidth) / 2;

constexpr int timerInnerX = padding + TIMER_BAR_THICKNESS;
constexpr int timerInnerY = QRCodeWidth + padding * 2 + TIMER_BAR_THICKNESS;
constexpr int timerBarInnerWidth = QRCodeWidth - (TIMER_BAR_THICKNESS * 2);
constexpr int timerBarInnerHeight = TIMER_BAR_HEIGHT - (TIMER_BAR_THICKNESS * 2);

MCUFRIEND_kbv tft;

int timerValue = TIMER_INACTIVE;
int timerProgressX = timerInnerX;

int idleTextX = IDLE_TEXT_INACTIVE;
int idleTextY = IDLE_TEXT_INACTIVE;
int idleTextDx = IDLE_TEXT_INACTIVE;
int idleTextDy = IDLE_TEXT_INACTIVE;

const PROGMEM int idleTextColors[] = { TFT_RED, TFT_BLUE, TFT_GREEN, TFT_YELLOW };
int idleTextColorsIndex = 0;
constexpr int idleTextColorsLength = 4;

int idleTextWidth, idleTextHeight;

void setupDisplay() {
    uint16_t ID = tft.readID();
    tft.begin(ID);

    // dummy variables
    int x1, y1;
    tft.setTextSize(5);
    tft.getTextBounds(IDLE_TEXT, 0, 0, &x1, &y1, &idleTextWidth, &idleTextHeight);

    clearScreen(TFT_BLACK);
}

void clearScreen(int color) {
    // reset both screens' variables
    timerValue = TIMER_INACTIVE;
    idleTextX = IDLE_TEXT_INACTIVE;
    idleTextY = IDLE_TEXT_INACTIVE;
    idleTextDx = IDLE_TEXT_INACTIVE;
    idleTextDy = IDLE_TEXT_INACTIVE;

    tft.fillScreen(color);
}

void updateScreen() {
    updateTimerBar();
    updateIdleText();
}

void displayHorizontallyCenteredText(char* text, int y) {
    int x1, y1, w, h;

    tft.setTextSize(3);
    tft.setTextColor(TFT_BLACK);
    tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
    tft.setCursor((SCREEN_WIDTH - w) / 2, y);
    tft.print(text);
}

void displayPaymentScreen(double amount) {
    if (timerValue != TIMER_INACTIVE) {
        return;
    }

    clearScreen(TFT_WHITE);

    char* code = generateCode(amount);
    displayQRCode(code);
    free(code);

    displayTimerBar();
    displayAmount(amount);
}

void displayQRCode(char* code) {
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(QR_VERSION)];
    qrcode_initText(&qrcode, qrcodeData, QR_VERSION, ECC_LOW, code);

    int x = padding;
    int y = padding;
    for (int i = 0; i < qrcode.size; i++) {
        for (int j = 0; j < qrcode.size; j++) {
            if (qrcode_getModule(&qrcode, j, i)) {
                tft.fillRect(
                    x + j * QR_PIXEL_SIZE, y + i * QR_PIXEL_SIZE,
                    QR_PIXEL_SIZE, QR_PIXEL_SIZE,
                    TFT_BLACK
                );
            }
        }
    }
}

void displayTimerBar() {
    int x = padding;
    int y = QRCodeWidth + padding * 2;
    for (int i = 0; i < TIMER_BAR_THICKNESS; i++) {
        tft.drawRect(
            x + i, y + i,
            QRCodeWidth - (i * 2), TIMER_BAR_HEIGHT - (i * 2),
            TFT_BLACK
        );
    }

    timerValue = 0;
    timerProgressX = timerInnerX;
}

void updateTimerBar() {
    if (timerValue == TIMER_INACTIVE) {
        return;
    }

    timerValue++;
    if (timerValue >= TIMER_DURATION) {
        timerValue = TIMER_INACTIVE;
        timerProgressX = timerInnerX;

        reboot();
        return;
    }

    double elapsed = (double) timerValue / TIMER_DURATION;
    int newTimerProgressX = timerInnerX + timerBarInnerWidth * elapsed;
    tft.fillRect(
        timerProgressX, timerInnerY,
        newTimerProgressX - timerProgressX, timerBarInnerHeight,
        TFT_BLUE
    );

    timerProgressX = newTimerProgressX;
}

void displayAmount(double amount) {
    // char* roundedAmount = (char*) malloc(10 * sizeof(char));
    char roundedAmount[8];
    int integral = (int) amount;
    int decimal = (int) (amount * 100) % 100;
    // sprintf_P(roundedAmount, PSTR("$%d.%02d"), integral, decimal);
    sprintf(roundedAmount, "$%d.%02d", integral, decimal);

    int y = QRCodeWidth + TIMER_BAR_HEIGHT + padding * 3;
    displayHorizontallyCenteredText(roundedAmount, y);
}

void displayIdleScreen() {
    clearScreen(TFT_BLACK);

    idleTextX = 0;
    idleTextY = 0;
    idleTextDx = IDLE_TEXT_VELOCITY;
    idleTextDy = IDLE_TEXT_VELOCITY;

    // clearCart();
}

void toggleIdleTextColor() {
    idleTextColorsIndex = (idleTextColorsIndex + 1) % idleTextColorsLength;
}

void updateIdleText() {
    if (idleTextX == IDLE_TEXT_INACTIVE) {
        return;
    }

    // overwrite previous
    tft.fillRect(idleTextX, idleTextY, idleTextWidth, idleTextHeight, TFT_BLACK);

    idleTextX += idleTextDx;
    idleTextY += idleTextDy;
    if (idleTextX < 0) {
        idleTextX = 0;
        idleTextDx *= -1;
        toggleIdleTextColor();
    }
    if (idleTextY < 0) {
        idleTextY = 0;
        idleTextDy *= -1;
        toggleIdleTextColor();
    }
    if (idleTextX + idleTextWidth > SCREEN_WIDTH) {
        idleTextX = SCREEN_WIDTH - idleTextWidth;
        idleTextDx *= -1;
        toggleIdleTextColor();
    }
    if (idleTextY + idleTextHeight > SCREEN_HEIGHT) {
        idleTextY = SCREEN_HEIGHT - idleTextHeight;
        idleTextDy *= -1;
        toggleIdleTextColor();
    }

    tft.setTextSize(5);
    tft.setTextColor(pgm_read_word_near(idleTextColors + idleTextColorsIndex));
    tft.setCursor(idleTextX, idleTextY);
    tft.print(IDLE_TEXT);
}
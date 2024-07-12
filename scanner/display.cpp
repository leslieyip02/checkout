#include <stdint.h>

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "qrcode.h"

#include "config.h"
#include "display.h"
#include "paynow.h"

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
int idleTextWidth, idleTextHeight;
int idleTextColors[] = {
    TFT_WHITE, TFT_RED, TFT_BLUE, TFT_GREEN,
    TFT_YELLOW, TFT_ORANGE, TFT_PURPLE, TFT_PINK
};
int idleTextColorsIndex = 0;
int idleTextColorsLength = 8;

void setupDisplay() {
    uint16_t ID = tft.readID();
    tft.begin(ID);

    // dummy variables
    int x1, y1;
    tft.setTextSize(3);
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
    clearScreen(TFT_WHITE);

    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(QR_VERSION)];

    char* code = generateCode(amount);
    qrcode_initText(&qrcode, qrcodeData, QR_VERSION, ECC_LOW, code);
    displayQRCode(&qrcode, QR_VERSION);
    displayTimerBar();
    displayAmount(amount);

    free(code);
}

void displayQRCode(QRCode* qrcode, int version) {
    // qr code
    int x = padding;
    int y = padding;
    for (int i = 0; i < qrcode->size; i++) {
        for (int j = 0; j < qrcode->size; j++) {
            if (qrcode_getModule(qrcode, j, i)) {
                tft.fillRect(
                    x + j * QR_PIXEL_SIZE,
                    y + i * QR_PIXEL_SIZE,
                    QR_PIXEL_SIZE,
                    QR_PIXEL_SIZE,
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
            x + i,
            y + i,
            QRCodeWidth - (i * 2),
            TIMER_BAR_HEIGHT - (i * 2),
            TFT_BLACK
        );
    }

    timerValue = 0;
}

void updateTimerBar() {
    if (timerValue == TIMER_INACTIVE) {
        return;
    }

    timerValue++;
    if (timerValue >= TIMER_DURATION) {
        timerValue = TIMER_INACTIVE;
        timerProgressX = timerInnerX;

        displayIdleScreen();
        return;
    }

    double elapsed = (double) timerValue / TIMER_DURATION;
    int newTimerProgressX = timerInnerX + timerBarInnerWidth * elapsed;
    tft.fillRect(
        timerProgressX,
        timerInnerY,
        newTimerProgressX - timerProgressX,
        timerBarInnerHeight,
        TFT_BLUE
    );
    timerProgressX = newTimerProgressX;
}

void displayAmount(double amount) {
    char* roundedAmount = (char*) malloc(16 * sizeof(char));
    int integral = (int) amount;
    int decimal = (int) (amount * 100) % 100;
    sprintf(roundedAmount, "Total: $%d.%02d", integral, decimal);

    int y = QRCodeWidth + TIMER_BAR_HEIGHT + padding * 3;
    displayHorizontallyCenteredText(roundedAmount, y);

    free(roundedAmount);
}

void displayIdleScreen() {
    clearScreen(TFT_BLACK);

    idleTextX = 0;
    idleTextY = 0;
    idleTextDx = IDLE_TEXT_VELOCITY;
    idleTextDy = IDLE_TEXT_VELOCITY;
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

    tft.setTextSize(3);
    tft.setTextColor(idleTextColors[idleTextColorsIndex]);
    tft.setCursor(idleTextX, idleTextY);
    tft.print(IDLE_TEXT);
}
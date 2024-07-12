#include <stdint.h>

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "qrcode.h"

#include "config.h"
#include "display.h"

constexpr int QRCodeWidth = (4 * QR_VERSION + 17) * QR_PIXEL_SIZE;
constexpr int padding = (SCREEN_WIDTH - QRCodeWidth) / 2;

constexpr int timerInnerX = padding + TIMER_BAR_THICKNESS;
constexpr int timerInnerY = QRCodeWidth + padding * 2 + TIMER_BAR_THICKNESS;
constexpr int timerBarInnerWidth = QRCodeWidth - (TIMER_BAR_THICKNESS * 2);
constexpr int timerBarInnerHeight = TIMER_BAR_HEIGHT - (TIMER_BAR_THICKNESS * 2);

MCUFRIEND_kbv tft;

int timerValue = TIMER_INACTIVE;
int timerProgressX = timerInnerX;

void setupDisplay() {
    uint16_t ID = tft.readID();
    tft.begin(ID);
    clearScreen();
}

void clearScreen() {
    tft.fillScreen(TFT_WHITE);
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
        clearScreen();
        return;
    }

    delay(10);
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

void displayHorizontallyCenteredText(char* text, int y) {
    int x1, y1, w, h;

    tft.setTextSize(3);
    tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
    tft.setCursor((SCREEN_WIDTH - w) / 2, y);
    tft.setTextColor(TFT_BLACK);
    tft.print(text);
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
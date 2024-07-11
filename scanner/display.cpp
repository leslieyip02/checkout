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
    tft.fillScreen(WHITE);
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
                    BLACK
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
            BLACK
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
        BLUE
    );
    timerProgressX = newTimerProgressX;
}
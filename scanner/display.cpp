#include <stdint.h>

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "qrcode.h"

#include "config.h"
#include "display.h"

MCUFRIEND_kbv tft;

void setupDisplay() {
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.fillScreen(WHITE);
}

void displayQRCode(QRCode* qrcode, int version) {
    int totalWidth = (4 * QR_VERSION + 17) * QR_PIXEL_SIZE;
    int x = (SCREEN_WIDTH - totalWidth) / 2;
    int y = 8;
    for (int j = 0; j < qrcode->size; j++) {
        for (int i = 0; i < qrcode->size; i++) {
            if (qrcode_getModule(qrcode, i, j)) {
                tft.fillRect(x + i * QR_PIXEL_SIZE, y + j * QR_PIXEL_SIZE,
                    QR_PIXEL_SIZE, QR_PIXEL_SIZE, BLACK);
            }
        }
    }
}
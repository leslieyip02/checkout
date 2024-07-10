#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "qrcode.h"

#include "config.h"
#include "constants.h"
#include "paynow.c"

MCUFRIEND_kbv tft;

void setup() {
  uint16_t ID = tft.readID();
  Serial.begin(9600);
  // Serial.println(ID);

  tft.begin(ID);
  tft.fillScreen(WHITE);

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(QR_VERSION)];

  char* code = generateCode(3.00);
  // Serial.println(code);
  qrcode_initText(&qrcode, qrcodeData, QR_VERSION, ECC_LOW, code);
  displayQRCode(&qrcode, QR_VERSION);
}

void loop() {}

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
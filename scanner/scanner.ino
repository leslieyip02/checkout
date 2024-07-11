#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "qrcode.h"

#include "config.h"
#include "display.h"
#include "paynow.h"

void setup() {
    // debugging
    // Serial.begin(9600);

    setupDisplay();

    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(QR_VERSION)];

    char* code = generateCode(3.2161);
    qrcode_initText(&qrcode, qrcodeData, QR_VERSION, ECC_LOW, code);
    displayQRCode(&qrcode, QR_VERSION);
    displayTimerBar();
}

void loop() {
    updateTimerBar();
}
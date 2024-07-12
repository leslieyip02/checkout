#include <stdlib.h>

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

    double amount = 3.2161;
    char* code = generateCode(amount);
    qrcode_initText(&qrcode, qrcodeData, QR_VERSION, ECC_LOW, code);
    displayQRCode(&qrcode, QR_VERSION);
    displayTimerBar();
    displayAmount(amount);

    free(code);
}

void loop() {
    updateTimerBar();
}
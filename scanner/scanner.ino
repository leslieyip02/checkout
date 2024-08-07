#include "parser.h"

void setup() {
    setupScanner();
    pinMode(PAYMENT_BUTTON_PIN, INPUT);
    pinMode(CANCEL_BUTTON_PIN, INPUT);
}

void loop() {
    if (digitalRead(PAYMENT_BUTTON_PIN) == HIGH) {
        sendMessage(PAYMENT_MESSAGE);
        delay(500);
    } else if (digitalRead(CANCEL_BUTTON_PIN) == HIGH) {
        sendMessage(CANCEL_MESSAGE);
        delay(500);
    }

    usb.Task();
    delay(10);
}
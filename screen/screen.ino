#include "config.h"
#include "display.h"

double amount = 12.34;

void setup() {
    // debugging
    // Serial.begin(115200);

    // pinMode(TOGGLE_BUTTON_PIN, INPUT);

    setupDisplay();
    displayIdleScreen();
    // displayPaymentScreen(amount);
}

void loop() {
    // int buttonState = digitalRead(TOGGLE_BUTTON_PIN);
    // if (buttonState == HIGH) {
    //     if (isQRCodeScreen()) {
    //         displayIdleScreen();
    //     } else {
    //         displayPaymentScreen(amount);
    //     }
    // }

    updateScreen();
    delay(10);
}
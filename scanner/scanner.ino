#include "config.h"
#include "display.h"

double amount = 12.34;

void setup() {
    // debugging
    // Serial.begin(9600);

    pinMode(TOGGLE_BUTTON_PIN, INPUT);

    setupDisplay();
    displayIdleScreen();
}

void loop() {
    int buttonState = digitalRead(TOGGLE_BUTTON_PIN);
    if (buttonState == HIGH) {
        if (isQRCodeScreen()) {
            displayIdleScreen();
        } else {
            displayPaymentScreen(amount);
        }
    }

    updateScreen();
    delay(10);
}
#include "config.h"
#include "display.h"

void setup() {
    // debugging
    // Serial.begin(9600);

    setupDisplay();
    displayPaymentScreen(3.2161);
}

void loop() {
    updateScreen();
    delay(10);
}
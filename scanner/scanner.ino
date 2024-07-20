#include "parser.h"

void setup() {
    // debugging
    Serial.begin(115200);

    setupScanner();
}

void loop() {
    usb.Task();
    delay(10);
}
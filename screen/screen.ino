#include "config.h"
#include "display.h"
#include "record.h"

void setup() {
    // debugging via Serial is not available
    // since it is used to communicate with the scanner

    setupDisplay();
    setupScanListener();
    displayIdleScreen();
}

void loop() {
    listenForScans();
    updateScreen();
    delay(10);
}
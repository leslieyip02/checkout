#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <avr/wdt.h>
#include <SoftwareSerial.h>
#include <SD.h>

#include "display.h"
#include "record.h"

double total = 0.0;

void setupScanListener() {
    // the 3.5" TFT LCD takes 8 pins for display
    // and 4 pins for its SD card
    // so there are no other pins available on the UNO
    Serial.begin(9600);
    SD.begin(SD_CS);

    // clearCart();
}

void listenForScans() {
    if (Serial.available() > 0) {
        char current = (char) Serial.read();
        while (Serial.available() > 0 && current != MESSAGE_START) {
            current = (char) Serial.read();
        }
        if (Serial.available() == 0) {
            return;
        }

        char* buffer = (char*) malloc((BARCODE_LENGTH + 1) * sizeof(char));
        char* bufferPointer = buffer;
        current = (char) Serial.read();
        while (current != MESSAGE_END) {
            if (isalnum(current)) {
                *bufferPointer = current;
                bufferPointer++;
            }
            current = (char) Serial.read();
        }
        *bufferPointer = '\0';

        if (strcmp_P(buffer, PAYMENT_MESSAGE) == 0) {
            displayPaymentScreen(total);
        } else if (strcmp_P(buffer, CANCEL_MESSAGE) == 0) {
            reboot();
        } else {
            addToCart(buffer);
        }

        free(buffer);
    }
}

void addToCart(char* scannedBarcode) {
    // SD card is read for every scan to save memory
    File pricesFile = SD.open(PRICES_CSV, FILE_READ);
    char* lineBuffer = (char*) malloc(MAX_CSV_LINE_LENGTH * sizeof(char));
    readCSVLine(pricesFile, lineBuffer);

    // mom: we have hashmaps at home
    // hashmaps at home:
    char* referenceBarcode = (char*) malloc((BARCODE_LENGTH + 1) * sizeof(char));
    while (pricesFile.available() > 0) {
        // ignore empty lines
        if (!readCSVLine(pricesFile, lineBuffer)) {
            continue;
        }

        // CSV file format
        // barcode       | price
        // ---------------------
        // XXXXXXXXXXXXX | XXX

        int offset = 0;
        while (offset < BARCODE_LENGTH && *(lineBuffer + offset) != ',') {
            *(referenceBarcode + offset) = *(lineBuffer + offset);
            offset++;
        }
        *(referenceBarcode + offset) = '\0';

        if (strcmp(scannedBarcode, referenceBarcode) == 0) {
            double price = atof(lineBuffer + offset + 1);
            total += price;
            break;
        }
    }

    free(referenceBarcode);
    free(lineBuffer);
    pricesFile.close();
}

// void clearCart() {
//     total = 0.0;
// }

bool readCSVLine(File& file, char* buffer) {
    char* bufferPointer = buffer;
    char current = file.read();
    bool empty = true;
    while (file.available() > 0 && current != '\n') {
        *bufferPointer = current;
        bufferPointer++;
        current = file.read();
        empty = false;
    }
    *bufferPointer = '\0';
    return !empty;
}

// lazy way to reset
// since memory errors might occur
void reboot() {
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1) {}
}
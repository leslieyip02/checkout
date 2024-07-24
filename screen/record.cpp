#include <ctype.h>
#include <string.h>

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "display.h"
#include "record.h"

int* quantities;
char* itemCodes[] = { ITEM_1_BARCODE, ITEM_2_BARCODE };
double itemCosts[] = { ITEM_1_COST, ITEM_2_COST };

void setupScanListener() {
    // the 3.5" TFT LCD takes 8 pins for display
    // and 4 pins for its SD card
    // so there are no other pins available on the UNO
    Serial.begin(9600);

    quantities = (int*) malloc(NUM_ITEMS * sizeof(int));
    clearCart();
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

        char* barcodeBuffer = (char*) malloc(14 * sizeof(char));
        char* bufferPointer = barcodeBuffer;
        current = (char) Serial.read();
        while (current != MESSAGE_END) {
            if (isalnum(current)) {
                *bufferPointer = current;
                bufferPointer++;
            }
            current = (char) Serial.read();
        }
        *bufferPointer = '\0';

        if (strcmp(barcodeBuffer, PAYMENT_MESSAGE) == 0) {
            double amount = tallyAmount();
            displayPaymentScreen(amount);
            clearCart();
        } else if (strcmp(barcodeBuffer, CANCEL_MESSAGE) == 0) {
            displayIdleScreen();
            clearCart();
        } else {
            addToCart(barcodeBuffer);
        }

        free(barcodeBuffer);
    }
}

void addToCart(char* barcode) {
    for (int i = 0; i < NUM_ITEMS; i++) {
        if (strcmp(barcode, itemCodes[i]) == 0) {
            quantities[i]++;
            return;
        }
    }
}

void clearCart() {
    // mom: we have memset at home
    // memset at home:
    for (int i = 0; i < NUM_ITEMS; i++) {
        quantities[i] = 0;
    }
}

double tallyAmount() {
    double total = 0.0;

    // mom: we have hashmaps at home
    // hashmaps at home:
    for (int i = 0; i < NUM_ITEMS; i++) {
        total += quantities[i] * itemCosts[i];
    }

    return total;
}
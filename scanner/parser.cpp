#include <stdlib.h>
#include <string.h>

#include <usbhid.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <SPI.h>

#include "parser.h"

USB usb;
HIDUniversal hid(&usb);
Parser scanner;

Parser::Parser() {
    itemsOrdered = (int*) malloc(1 * sizeof(int));
    ClearBuffer();
}

void Parser::Parse(USBHID* hid, bool is_rpt_id, uint8_t len, uint8_t* buf) {
    // if buffer contains error or is empty, skip
    if (buf[2] == 1 || buf[2] == 0) {
        return;
    }

    for (uint8_t i = 7; i >= 2; i--) {
        if (buf[i] == 0) {
            continue;
        }

        // if enter signal emitted, scan finished
        if (buf[i] == UHS_HID_BOOT_KEY_ENTER) {
            OnScanFinished();
        } else {
            // if bit position not in 2, it's uppercase
            OnKeyScanned(i > 2, buf[i]);
        }

        return;
    }
}

uint8_t Parser::KeyToAscii(bool upper, uint8_t key) {
    // Letters
    if (VALUE_WITHIN(key, 0x04, 0x1d)) {
        return key - 4 + upper ? 'A' : 'a';
    }
    if (VALUE_WITHIN(key, 0x1e, 0x27)) {
        return key == UHS_HID_BOOT_KEY_ZERO ? '0' : key - 0x1e + '1';
    }
    return 0;
}

void Parser::OnKeyScanned(bool upper, uint8_t key) {
    uint8_t ascii = KeyToAscii(upper, key);
    *bufferPointer = (char) ascii;
    bufferPointer++;
}

void Parser::OnScanFinished() {
    *bufferPointer = '\0';

    char* dummyId = "0100000010174";
    int dummyIndex = 0;
    if (strcmp(bufferPointer, dummyId) == 0) {
        itemsOrdered[dummyIndex]++;
    };

    Serial.println(barcodeBuffer);
    ClearBuffer();
}

void Parser::ClearBuffer() {
    free(barcodeBuffer);
    barcodeBuffer = (char*) malloc((BARCODE_LENGTH + 1) * sizeof(char));
    bufferPointer = barcodeBuffer;
}

void setupScanner() {
    usb.Init();
    hid.SetReportParser(0, &scanner);
}

double getAmount() {
    double amount = 0.0;
    for (int i = 0; i < 1; i++) {
        double itemCost = 1.23;
        double quantity = scanner.itemsOrdered[i];
        amount += itemCost * quantity;
    }
    return amount;
}
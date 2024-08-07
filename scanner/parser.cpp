#include <stdlib.h>
#include <string.h>

#include <SoftwareSerial.h>
#include <SPI.h>
#include <hidboot.h>
#include <hiduniversal.h>
#include <usbhid.h>

#include "parser.h"

USB usb;
HIDUniversal hid(&usb);
Parser scanner;

SoftwareSerial output(RX, TX);

void setupScanner() {
    usb.Init();
    hid.SetReportParser(0, &scanner);
    output.begin(9600);
}

void sendMessage(char* message) {
    output.write(MESSAGE_START);
    output.print(message);
    output.write(MESSAGE_END);
}

Parser::Parser() {
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
    sendMessage(barcodeBuffer);
    ClearBuffer();
}

void Parser::ClearBuffer() {
    free(barcodeBuffer);
    barcodeBuffer = (char*) malloc((BARCODE_LENGTH + 1) * sizeof(char));
    bufferPointer = barcodeBuffer;
}
#ifndef SCANNER_H
#define SCANNER_H

#include <usbhid.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <SPI.h>

// 13 characters is enough to fit EAN-13 retail barcodes
#define BARCODE_LENGTH 13

#define RX 2
#define TX 3
#define PAYMENT_BUTTON_PIN 4
#define CANCEL_BUTTON_PIN 5

#define MESSAGE_START '<'
#define MESSAGE_END '>'
#define PAYMENT_MESSAGE "PAYMENT"
#define CANCEL_MESSAGE "CANCEL"

extern USB usb;
extern void setupScanner();
extern void sendMessage(char* message);

// navigation
extern void startPayment();
extern void cancelPayment();

class Parser : public HIDReportParser {
public:
    Parser();
    void Parse(USBHID* hid, bool is_rpt_id, uint8_t len, uint8_t* buf);

private:
    char* barcodeBuffer;
    char* bufferPointer;

    uint8_t KeyToAscii(bool upper, uint8_t key);
    void OnKeyScanned(bool upper, uint8_t key);
    void OnScanFinished();
    void ClearBuffer();
};

#endif
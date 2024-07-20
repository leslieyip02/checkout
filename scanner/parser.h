#ifndef SCANNER_H
#define SCANNER_H

#include <usbhid.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <SPI.h>

// 13 characters is enough to fit EAN-13 retail barcodes
#define BARCODE_LENGTH 13

extern USB usb;
extern void setupScanner();
extern double getAmount();

class Parser : public HIDReportParser {
public:
    int* itemsOrdered;
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
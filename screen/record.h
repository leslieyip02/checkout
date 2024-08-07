#ifndef RECORD_H
#define RECORD_H

#include <avr/pgmspace.h>
#include <SD.h>

#define MESSAGE_START '<'
#define MESSAGE_END '>'
const char PAYMENT_MESSAGE[] PROGMEM = "PAY";
const char CANCEL_MESSAGE[] PROGMEM = "XXX";

#define SD_CS 10
#define PRICES_CSV F("p.csv")
#define MAX_CSV_LINE_LENGTH 20

// 13 characters is enough to fit EAN-13 retail barcodes
#define BARCODE_LENGTH 13

extern void setupScanListener();
extern void listenForScans();
void addToCart(char* scannedBarcode);
// extern void clearCart();
bool readCSVLine(File& file, char* buffer);
void reboot();

#endif
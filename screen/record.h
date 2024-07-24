#ifndef RECORD_H
#define RECORD_H

#define MESSAGE_START '<'
#define MESSAGE_END '>'
#define PAYMENT_MESSAGE "PAYMENT"
#define CANCEL_MESSAGE "CANCEL"

#define NUM_ITEMS 2

// TODO: replace hard coding
#define ITEM_1_BARCODE "100000010174"
#define ITEM_2_BARCODE "8888112013000"
#define ITEM_1_COST 1.00
#define ITEM_2_COST 1.23

extern void setupScanListener();
extern void listenForScans();

void addToCart(char* barcode);
extern void clearCart();
extern double tallyAmount();

#endif
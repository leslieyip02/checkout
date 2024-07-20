#ifndef PAYNOW_H
#define PAYNOW_H

// 000201           // format
// 010211           // method (11 for static, 12 for dynamic)
// 2638             // merchant account information header (38 is the length of the body)
// 0009SG.PAYNOW    // reverse domain name
// 01010            // proxy type (0 for mobile number, 2 for UEN)
#define BLOCK_1 "00020101021126380009SG.PAYNOW01010"

// 03011            // editability (0 for no, 1 for yes)
// 52040000         // merchant category
// 5303702          // currency (702 is the ISO4217 code for SGD)
#define BLOCK_2 "03011520400005303702"

// roundedAmount    // amount (2dp)
// 5802SG           // country code
// 5902NA           // merchant name
// 6009Singapore    // city
// 6211             // message header (11 is the length of the body)
#define BLOCK_3 "5802SG5902NA6009Singapore6211"

extern char* generateCode(double amount);
extern char* generateChecksum(char* payload);

#endif
#ifndef PAYNOW_H
#define PAYNOW_H

#include <avr/pgmspace.h>

// each segment of the code is structure as follows
// <ID><length><value>

// 000201               format
// 010211               method (11 for static, 12 for dynamic)
// 2638                 merchant account information header (38 is the length of the body)
// 0009SG.PAYNOW        reverse domain name
// 01010                proxy type (0 for mobile number, 2 for UEN)
// 0211+6597223615      proxy value (+65XXXXXXXX)
// 03011                editability (0 for no, 1 for yes)
// 52040000             merchant category
// 5303702              currency (702 is the ISO4217 code for SGD)
// 53406                amount (2dp)
const char BLOCK_1[] PROGMEM = "00020101021126380009SG.PAYNOW010100211+6597223615030115204000053037025406";

// 5802SG               country code
// 5902NA               merchant name
// 6009Singapore        city
// 6211                 message header (11 is the length of the body)
// 0107RC4 SOS          message
// 6304                 checksum indicator
const char BLOCK_2[] PROGMEM = "5802SG5902NA6009Singapore62110107RC4 SOS6304";

extern char* generateCode(double amount);
extern char* generateChecksum(char* payload);

#endif
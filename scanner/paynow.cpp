#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "paynow.h"

char* generateCode(double amount) {
    // each segment of the code is structure as follows
    // <ID><length><value>
    char* payload = (char*) malloc(QR_CAPACITY * sizeof(char));

    char* proxyValue = (char*) malloc(16 * sizeof(char));
    sprintf(proxyValue, "0211%s", PAYNOW_PHONE_NUMBER);

    // sprintf does not work with floats on arduino
    char* roundedAmount = (char*) malloc(16 * sizeof(char));
    int integral = (int) amount;
    int decimal = (int) (amount * 100) % 100;
    sprintf(roundedAmount, "5406%03d.%02d", integral, decimal);

    char* message = (char*) malloc(16 * sizeof(char));
    sprintf(message, "01%02d%s", strlen(PAYNOW_MESSAGE), PAYNOW_MESSAGE);

    strcpy(payload, "000201");          // format
    strcat(payload, "010211");          // method (11 for static, 12 for dynamic)

    strcat(payload, "2638");            // merchant account information header (38 is the length of the body)
    strcat(payload, "0009SG.PAYNOW");   // reverse domain name
    strcat(payload, "01010");           // proxy type (0 for mobile number, 2 for UEN)
    strcat(payload, proxyValue);        // proxy value (+65XXXXXXXX)
    strcat(payload, "03011");           // editability (0 for no, 1 for yes)

    strcat(payload, "52040000");        // merchant category
    strcat(payload, "5303702");         // currency (702 is the ISO4217 code for SGD)
    strcat(payload, roundedAmount);     // amount (2dp)
    strcat(payload, "5802SG");          // country code
    strcat(payload, "5902NA");          // merchant name
    strcat(payload, "6009Singapore");   // city
    strcat(payload, "6211");            // message header (11 is the length of the body)
    strcat(payload, message);           // message
    strcat(payload, "6304");            // checksum indicator

    char* checksum = generateChecksum(payload);
    strcat(payload, checksum);
    return payload;
}

char* generateChecksum(char* payload) {
    // CRC16-CCITT checksum calculation
    uint16_t crc = 0xFFFF;
    while (*payload != '\0') {
        uint16_t mask = crc >> 8;
        uint8_t x = mask ^ *payload;
        x ^= (x >> 4);
        crc = (crc << 8) ^ ((uint16_t) (x << 12)) ^ ((uint16_t) (x << 5)) ^ ((uint16_t) x);
        payload++;
    }

    // split 16-bit crc into 4-bit chunks and convert to hexadecimal string
    char* checksum = (char*) malloc(5 * sizeof(char));
    for (int i = 0; i < 4; i++) {
        uint16_t mask = 0xF000 >> (i * 4);
        uint16_t nibble = (mask & crc) >> ((3 - i) * 4);
        *(checksum + i) = nibble + (nibble >= 10 ? 55 : '0');
    }
    *(checksum + 4) = '\0';
    return checksum;
}
// #include <math.h>
// #include <stdint.h>
// #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Arduino.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "paynow.h"

char* generateCode(double amount) {
    int integral = (int) amount;
    int decimal = (int) (amount * 100) % 100;

    char* payload = (char*) malloc(QR_CAPACITY * sizeof(char));
    sprintf_P(
        payload, PSTR("%S%03d.%02d%S"),
        BLOCK_1, integral, decimal, BLOCK_2
    );

    char* checksum = generateChecksum(payload);
    strcat(payload, checksum);
    free(checksum);

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